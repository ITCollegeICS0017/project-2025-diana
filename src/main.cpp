
#include "Config.h"
#include "Util.h"
#include "Ticket.h"
#include "Repositories.h"
#include "TicketService.h"
#include "ConsoleUI.h"
#include "Employee.h"
#include "IClock.h"
#include "FileTicketRepository.h"
#include "Errors.h"

#include <iostream>
#include <memory>
#include <functional>

std::unique_ptr<IClock> makeSimpleClock();

int main() {
    try {
        const std::string ticketsFile = "tickets.csv";
        const std::string passengersFile = "passengers.csv";
        const std::string registryFile = "transactions.csv";

        TicketRepository ticketRepo;
        PassengerRepository passengerRepo;
        auto clock = makeSimpleClock();
        TicketService ticketService(ticketRepo, passengerRepo, *clock);

        // --- Load tickets ---
        try {
            FileTicketRepository fileTicketRepo(ticketsFile);
            fileTicketRepo.load();
            for (const auto& t : fileTicketRepo.listAll()) ticketRepo.addTicket(t);
            std::cout << "Loaded tickets from " << ticketsFile << "\n";
        }
        catch (...) {
            std::cout << "Tickets file missing; bootstrapping defaults...\n";
            ticketRepo.addTicket(Ticket(1, "2025-10-01", 50.0f, "Tallinn", Coach::Sleeper, Status::Available));
            ticketRepo.addTicket(Ticket(2, "2025-10-02", 30.0f, "Riga", Coach::Compartment, Status::Available));
            ticketRepo.addTicket(Ticket(3, "2025-10-01", 20.0f, "Tallinn", Coach::Economy, Status::Available));
        }

        // --- Load passengers ---
        try {
            passengerRepo.load(passengersFile);
            std::cout << "Loaded passengers from " << passengersFile << "\n";
        }
        catch (...) {
            std::cout << "Passengers file missing; bootstrapping defaults...\n";
            passengerRepo.addPassenger("John", 100.0f);
            passengerRepo.save(passengersFile);
        }

        // --- Load transactions ---
        try {
            ticketService.loadRegistry(registryFile);
            std::cout << "Loaded transactions from " << registryFile << "\n";
        }
        catch (...) {
            std::cout << "No transaction registry found; starting fresh.\n";
        }

        Clerk clerk("Alice");
        Administrator admin("Bob");
        std::cout << "Employee created: " << clerk.name() << " role: " << clerk.role() << "\n";
        std::cout << "Employee created: " << admin.name() << " role: " << admin.role() << "\n";

        ConsoleUI ui(ticketRepo, passengerRepo, ticketService);

        // ✅ Auto-save callback for passenger changes
        ui.setOnPassengerDataChanged([&] {
            try {
                passengerRepo.save(passengersFile);
                std::cout << "[Auto-Save] Passengers saved to " << passengersFile << "\n";
            }
            catch (const RepositoryException& ex) {
                std::cerr << "[Auto-Save] Passenger save error: " << ex.what() << "\n";
            }
            catch (const std::exception& ex) {
                std::cerr << "[Auto-Save] Unexpected save error: " << ex.what() << "\n";
            }
            });

        ui.run();

        // --- Save tickets on shutdown ---
        try {
            FileTicketRepository fileTicketRepo(ticketsFile);
            for (const auto& t : ticketRepo.listAll()) fileTicketRepo.add(t);
            fileTicketRepo.save();
            std::cout << "Saved tickets to " << ticketsFile << "\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "Ticket save error: " << ex.what() << "\n";
        }

        // --- Save transactions on shutdown ---
        try {
            ticketService.saveRegistry(registryFile);
            std::cout << "Saved transactions to " << registryFile << "\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "Registry save error: " << ex.what() << "\n";
        }

        std::cout << "Exiting application.\n";
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
}
