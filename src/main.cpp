#include "Repositories.h"
#include "TicketService.h"
#include "ConsoleUI.h"
#include "Employee.h"
#include "IClock.h"
#include "Errors.h"

#include <iostream>
#include <unordered_map>

std::unique_ptr<IClock> makeSimpleClock();

// Release 4: reconstruction rules
static void reconstructState(TicketRepository& ticketRepo, const TicketService& service) {
    // Rule A: any Reserved tickets must not survive restart (temporary block)
    for (int i = 0;; ++i) {
        Ticket* t = ticketRepo.getByIndex(i);
        if (!t) break;
        if (t->getStatus() == Status::Reserved) {
            t->setStatus(Status::Available);
        }
    }

    // Rule B: ticket status rebuilt from the last transaction per ticket
    // last op "purchase" => Sold
    // last op "return"   => Available
    std::unordered_map<int, std::string> lastOp;
    for (const auto& tx : service.getRegistry()) {
        lastOp[tx.ticketId] = tx.operation; // assumes registry is in chronological order (append-only)
    }

    for (const auto& kv : lastOp) {
        const int ticketId = kv.first;
        const std::string& op = kv.second;

        if (op == "purchase") {
            ticketRepo.updateStatusById(ticketId, Status::Sold);
        }
        else if (op == "return") {
            ticketRepo.updateStatusById(ticketId, Status::Available);
        }
        // ignore unknown ops safely
    }
}

int main() {
    const std::string ticketsFile = "tickets.csv";
    const std::string passengersFile = "passengers.csv";
    const std::string registryFile = "transactions.csv";

    try {
        // --- repositories & services ---
        TicketRepository ticketRepo;
        PassengerRepository passengerRepo;

        ticketRepo.setStoragePath(ticketsFile);

        auto clock = makeSimpleClock();
        TicketService service(ticketRepo, passengerRepo, *clock);

        // --- load tickets ---
        try {
            ticketRepo.load();
            std::cout << "Loaded tickets from " << ticketsFile << "\n";
        }
        catch (const std::exception& ex) {
            std::cout << "Tickets load failed.\n";
        }

        // --- load passengers ---
        try {
            passengerRepo.load(passengersFile);
            std::cout << "Loaded passengers from " << passengersFile << "\n";
        }
        catch (const std::exception& ex) {
            std::cout << "Passengers load failed (" << ex.what() << "). Starting empty.\n";
            // It's OK to start empty; file will be created on first save
        }

        // --- load transaction registry ---
        try {
            service.loadRegistry(registryFile);
            std::cout << "Loaded transactions from " << registryFile << "\n";
        }
        catch (const std::exception&) {
            std::cout << "No transaction registry found; starting fresh.\n";
        }

        // --- Release 4: reconstruction rules ---
        reconstructState(ticketRepo, service);

        // Persist reconstructed state
        try {
            ticketRepo.save();
            passengerRepo.save(passengersFile);
            service.saveRegistry(registryFile);
            std::cout << "Reconstruction applied and state saved.\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "Warning: failed to save reconstructed state: " << ex.what() << "\n";
        }

        Clerk clerk("Alice");
        Administrator admin("Bob");
        std::cout << "Employee created: " << clerk.name() << " role: " << clerk.role() << "\n";
        std::cout << "Employee created: " << admin.name() << " role: " << admin.role() << "\n";

        // --- UI ---
        ConsoleUI ui(ticketRepo, passengerRepo, service);
        ui.setOnPassengerDataChanged([&] {
            try {
                passengerRepo.save(passengersFile);
                ticketRepo.save();
                service.saveRegistry(registryFile);
                std::cout << "[Auto-Save] Saved passengers, tickets, transactions.\n";
            }
            catch (const std::exception& ex) {
                std::cerr << "[Auto-Save] Error: " << ex.what() << "\n";
            }
            });

        ui.run();

        // Save once more on shutdown
        try {
            passengerRepo.save(passengersFile);
            ticketRepo.save();
            service.saveRegistry(registryFile);
            std::cout << "Saved state on shutdown.\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "Shutdown save error: " << ex.what() << "\n";
        }

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
}
