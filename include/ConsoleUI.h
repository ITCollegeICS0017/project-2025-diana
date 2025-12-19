#pragma once
#include "Repositories.h"
#include "TicketService.h"
#include "Validation.h"
#include <functional>

class ConsoleUI {
public:
    ConsoleUI(TicketRepository& tr, PassengerRepository& pr, TicketService& svc);
    void setOnPassengerDataChanged(std::function<void()> cb);
    void run();

private:
    void showMainMenu();
    void listTickets();
    void searchAvailable();
    void registerPassenger();
    void purchaseFlow();
    void returnFlow();
    void showRegistryAndProfit();
    void viewPassengerProfile();
    void addFunds();

    void notifyPassengerDataChanged();

    TicketRepository& mTicketRepo;
    PassengerRepository& mPassengerRepo;
    TicketService& mService;

    std::function<void()> mOnPassengerDataChanged;
};
