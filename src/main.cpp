#include "Config.h"
#include "Util.h"
#include "Ticket.h"
#include "Repositories.h"
#include "TicketService.h"
#include "ConsoleUI.h"
#include "Employee.h"
#include "IClock.h"
#include <iostream>
#include <memory>

// Forward-declared factory from IClock.cpp
std::unique_ptr<IClock> makeSimpleClock();

int main() {
    TicketRepository ticketRepo;
    TrainRepository trainRepo;
    PassengerRepository passengerRepo;

    // sample tickets
    ticketRepo.addTicket(Ticket(1, "2025-10-01", 50.0f, "Tallinn", Coach::Sleeper, Status::Available));
    ticketRepo.addTicket(Ticket(2, "2025-10-02", 30.0f, "Riga", Coach::Compartment, Status::Available));
    ticketRepo.addTicket(Ticket(3, "2025-10-01", 20.0f, "Tallinn", Coach::Economy, Status::Available));

    passengerRepo.addPassenger("John", 100.0f);

    auto clock = makeSimpleClock();

    TicketService ticketService(ticketRepo, passengerRepo, *clock);

    Clerk clerk("Alice");
    Administrator admin("Bob");
    std::cout << "Employee created: " << clerk.name() << " role: " << clerk.role() << "\n";
    std::cout << "Employee created: " << admin.name() << " role: " << admin.role() << "\n\n";

    ConsoleUI ui(ticketRepo, passengerRepo, ticketService);
    ui.run();

    std::cout << "Exiting application.\n";
    return 0;
}
