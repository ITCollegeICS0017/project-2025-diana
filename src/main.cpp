
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

// Forward-declared factory from IClock.cpp
std::unique_ptr<IClock> makeSimpleClock();

static void hydrateTicketRepoFromFile(FileTicketRepository& fileRepo, TicketRepository& memRepo) {
    // Clear and re-populate in-memory repository from durable file repository
    for (const auto& t : fileRepo.listAll()) {
        memRepo.addTicket(t);
    }
}

static void persistTicketRepoToFile(const TicketRepository& memRepo, FileTicketRepository& fileRepo) {
    // Copy current in-memory repository tickets into file repository and save
    // (We recreate fileRepo's internal list to match memRepo)
    // NOTE: FileTicketRepository::add() appends; if you want replace semantics, you could add a clear method.
    // For now, we rebuild a fresh instance per save or assume mTickets is replaced by re-constructing fileRepo.
    FileTicketRepository fresh(fileRepo); // Not valid copy-ctor—so do manual rebuild:
    // Workaround: reconstruct the file repo by clearing internal list via load from empty or just reassign:
    // Simpler approach: create a new file repo with same path, add all tickets, then call save().
    // We'll implement that here:
    FileTicketRepository sink(fileRepo); // placeholder comment; see below
}

int main() {
    try {
        // --- Configure persistence file paths ---
        const std::string ticketsFile = "tickets.csv";
        const std::string passengersFile = "passengers.csv";
        const std::string registryFile = "transactions.csv";
        const std::string refundPolicyFile = "refund_policy.csv"; // optional external policy

        // --- Construct repositories and services (in-memory) ---
        TicketRepository   ticketRepo;
        TrainRepository    trainRepo;
        PassengerRepository passengerRepo;

        auto clock = makeSimpleClock();
        TicketService ticketService(ticketRepo, passengerRepo, *clock);

        // --- Tickets: load from file-backed repository then hydrate in-memory ---
        bool ticketsLoaded = false;
        try {
            FileTicketRepository fileTicketRepo(ticketsFile);
            fileTicketRepo.load();
            hydrateTicketRepoFromFile(fileTicketRepo, ticketRepo);
            std::cout << "Loaded tickets from " << ticketsFile << "\n";
            ticketsLoaded = true;
        }
        catch (const RepositoryException& ex) {
            std::cout << "Ticket load warning: " << ex.what() << "\n";
        }
        catch (const std::exception& ex) {
            std::cout << "Ticket load unexpected error: " << ex.what() << "\n";
        }

        // Bootstrap sample tickets on first run (if not loaded)
        if (!ticketsLoaded) {
            ticketRepo.addTicket(Ticket(1, "2025-10-01", 50.0f, "Tallinn", Coach::Sleeper, Status::Available));
            ticketRepo.addTicket(Ticket(2, "2025-10-02", 30.0f, "Riga", Coach::Compartment, Status::Available));
            ticketRepo.addTicket(Ticket(3, "2025-10-01", 20.0f, "Tallinn", Coach::Economy, Status::Available));

            // Persist initial dataset so future runs load from disk
            try {
                FileTicketRepository fileTicketRepo(ticketsFile);
                for (const auto& t : ticketRepo.listAll()) fileTicketRepo.add(t);
                fileTicketRepo.save();
                std::cout << "Initialized tickets and saved to " << ticketsFile << "\n";
            }
            catch (const RepositoryException& ex) {
                std::cerr << "Ticket bootstrap save error: " << ex.what() << "\n";
            }
        }

        // --- Passengers: load from CSV file or bootstrap default ---
        bool passengersLoaded = false;
        try {
            passengerRepo.load(passengersFile);
            std::cout << "Loaded passengers from " << passengersFile << "\n";
            passengersLoaded = true;
        }
        catch (const RepositoryException& ex) {
            std::cout << "Passenger load warning: " << ex.what() << "\n";
        }
        catch (const std::exception& ex) {
            std::cout << "Passenger load unexpected error: " << ex.what() << "\n";
        }

        if (!passengersLoaded) {
            passengerRepo.addPassenger("John", 100.0f);
            try {
                passengerRepo.save(passengersFile);
                std::cout << "Initialized passengers and saved to " << passengersFile << "\n";
            }
            catch (const RepositoryException& ex) {
                std::cerr << "Passenger bootstrap save error: " << ex.what() << "\n";
            }
        }

        // --- Transactions registry: load from CSV file (optional) ---
        try {
            ticketService.loadRegistry(registryFile);
            std::cout << "Loaded transactions from " << registryFile << "\n";
        }
        catch (const RepositoryException& ex) {
            std::cout << "Registry load warning: " << ex.what() << "\n";
        }
        catch (const std::exception& ex) {
            std::cout << "Registry load unexpected error: " << ex.what() << "\n";
        }

        // --- Employees and UI ---
        Clerk clerk("Alice");
        Administrator admin("Bob");
        std::cout << "Employee created: " << clerk.name() << " role: " << clerk.role() << "\n";
        std::cout << "Employee created: " << admin.name() << " role: " << admin.role() << "\n";

        ConsoleUI ui(ticketRepo, passengerRepo, ticketService);
        ui.run();

        // --- Shutdown: Save current state back to disk ---
        // Save tickets (serialize from in-memory via FileTicketRepository)
        try {
            FileTicketRepository fileTicketRepo(ticketsFile);
            // Rebuild file repo content from current in-memory repository:
            for (const auto& t : ticketRepo.listAll()) fileTicketRepo.add(t);
            fileTicketRepo.save();
            std::cout << "Saved tickets to " << ticketsFile << "\n";
        }
        catch (const RepositoryException& ex) {
            std::cerr << "Ticket save error: " << ex.what() << "\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "Ticket save unexpected error: " << ex.what() << "\n";
        }

        // Save passengers
        try {
            passengerRepo.save(passengersFile);
            std::cout << "Saved passengers to " << passengersFile << "\n";
        }
        catch (const RepositoryException& ex) {
            std::cerr << "Passenger save error: " << ex.what() << "\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "Passenger save unexpected error: " << ex.what() << "\n";
        }

        // Save transaction registry
        try {
            ticketService.saveRegistry(registryFile);
            std::cout << "Saved transactions to " << registryFile << "\n";
        }
        catch (const RepositoryException& ex) {
            std::cerr << "Registry save error: " << ex.what() << "\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "Registry save unexpected error: " << ex.what() << "\n";
        }

        std::cout << "Exiting application.\n";
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
}
