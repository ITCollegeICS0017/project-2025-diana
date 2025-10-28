#pragma once
#include "Repositories.h"
#include "TicketService.h"

class ConsoleUI {
public:
    ConsoleUI(TicketRepository& tr, PassengerRepository& pr, TicketService& svc);
    void run();

private:
    void showMainMenu();
    void listTickets();
    void searchAvailable();
    void registerPassenger();
    void purchaseFlow();
    void returnFlow();
    void showRegistryAndProfit();

    TicketRepository& mTicketRepo;
    PassengerRepository& mPassengerRepo;
    TicketService& mService;
};
