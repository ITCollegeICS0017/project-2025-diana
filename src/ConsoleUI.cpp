#include "ConsoleUI.h"
#include <iostream>
#include <string>
#include <optional>

ConsoleUI::ConsoleUI(TicketRepository& tr, PassengerRepository& pr, TicketService& svc)
    : mTicketRepo(tr), mPassengerRepo(pr), mService(svc) {}

void ConsoleUI::showMainMenu() {
    std::cout << "=== Railway Ticket Sales (CLI) ===\n";
    std::cout << "1) List Tickets\n";
    std::cout << "2) Search Available Tickets\n";
    std::cout << "3) Register Passenger\n";
    std::cout << "4) Purchase Ticket\n";
    std::cout << "5) Return Ticket\n";
    std::cout << "6) Show Registry & Profit\n";
    std::cout << "0) Exit\n";
}

void ConsoleUI::listTickets() {
    std::cout << "--- All Tickets ---\n";
    auto tlist = mTicketRepo.listAll();
    for (const auto& t : tlist) t.showDetails();
    int availableCount = 0;
    for (const auto& t : tlist) {
        if (t.getStatus() == Status::Available) ++availableCount;
    }
    Monitoring::checkTicketAvailability(availableCount);
}

void ConsoleUI::searchAvailable() {
    std::string dest, date;
    std::cout << "Destination (leave empty to match any): ";
    std::getline(std::cin, dest);
    std::cout << "Date (YYYY-MM-DD, leave empty to match any): ";
    std::getline(std::cin, date);
    std::cout << "Coach filter (Sleeper/Compartment/Economy/FirstClass or leave empty): ";
    std::string coachStr;
    std::getline(std::cin, coachStr);
    std::optional<Coach> cf = std::nullopt;
    if (!coachStr.empty()) {
        if (coachStr == "Sleeper") cf = Coach::Sleeper;
        else if (coachStr == "Compartment") cf = Coach::Compartment;
        else if (coachStr == "Economy") cf = Coach::Economy;
        else if (coachStr == "FirstClass") cf = Coach::FirstClass;
    }
    auto indices = mService.searchAvailable(dest, date, cf);
    if (indices.empty()) {
        std::cout << "No available tickets match the filters.\n";
        return;
    }
    std::cout << "Found tickets (repository indices):\n";
    for (int idx : indices) {
        Ticket* t = mTicketRepo.getByIndex(idx);
        if (t) t->showDetails();
    }
    int availableCount = 0;
    for (const auto& t : mTicketRepo.listAll()) {
        if (t.getStatus() == Status::Available) ++availableCount;
    }
    Monitoring::checkTicketAvailability(availableCount);

}

void ConsoleUI::registerPassenger() {
    std::string passport;
    std::cout << "Enter passport id (alnum, min 4 chars): ";
    std::getline(std::cin, passport);
    if (passport.size() < 4) {
        std::cout << "Invalid passport format.\n";
        return;
    }
    std::string balStr;
    std::cout << "Initial balance (EUR): ";
    std::getline(std::cin, balStr);
    float bal = 0.0f;
    try {
        bal = std::stof(balStr);
        if (bal < 0.0f) { std::cout << "Balance cannot be negative.\n"; return; }
    } catch (...) {
        std::cout << "Invalid amount.\n"; return;
    }
    mPassengerRepo.addPassenger(passport, bal);
    std::cout << "Passenger registered: " << passport << " balance: " << std::fixed << std::setprecision(2) << bal << "\n";
    Monitoring::checkBalance(bal);
}

void ConsoleUI::purchaseFlow() {
    std::string passport;
    std::cout << "Passenger passport: ";
    std::getline(std::cin, passport);
    std::string ticketIdStr;
    std::cout << "Ticket ID to purchase: ";
    std::getline(std::cin, ticketIdStr);
    int ticketId = std::stoi(ticketIdStr);
    std::string msg;
    bool ok = mService.completePurchase(passport, ticketId, msg);
    if (ok) {
        float newBalance = mPassengerRepo.getBalance(passport);
        Monitoring::checkBalance(newBalance);
    }
    std::cout << msg << "\n";
}

void ConsoleUI::returnFlow() {
    std::string passport;
    std::cout << "Passenger passport: ";
    std::getline(std::cin, passport);
    std::string ticketIdStr;
    std::cout << "Ticket ID to return: ";
    std::getline(std::cin, ticketIdStr);
    int ticketId = std::stoi(ticketIdStr);
    std::string msg;
    bool ok = mService.completeReturn(passport, ticketId, msg);
    std::cout << msg << "\n";
}

void ConsoleUI::showRegistryAndProfit() {
    auto regs = mService.getRegistry();
    std::cout << "--- Transaction Registry ---\n";
    for (const auto& r : regs) {
        std::cout << "Ticket " << r.ticketId << " " << r.operation << " " << r.timestamp << " amount: " << std::fixed << std::setprecision(2) << r.amount << "\n";
    }
    std::cout << "Daily profit: " << std::fixed << std::setprecision(2) << mService.dailyProfit() << " EUR\n";
}

void ConsoleUI::run() {
    while (true) {
        showMainMenu();
        std::cout << "Select option: ";
        std::string opt;
        std::getline(std::cin, opt);
        if (opt == "0") break;
        else if (opt == "1") listTickets();
        else if (opt == "2") searchAvailable();
        else if (opt == "3") registerPassenger();
        else if (opt == "4") purchaseFlow();
        else if (opt == "5") returnFlow();
        else if (opt == "6") showRegistryAndProfit();
        else std::cout << "Unknown option\n";
        std::cout << "\n";
    }
}
