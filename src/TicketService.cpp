#include "TicketService.h"
#include "Repositories.h"
#include "Util.h"
#include "Errors.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

TicketService::TicketService(TicketRepository& tr, PassengerRepository& pr, const IClock& clock)
    : mTicketRepo(tr), mPassengerRepo(pr), mClock(clock) {}

std::vector<int> TicketService::searchAvailable(const std::string& destination, const std::string& date, std::optional<Coach> coachFilter) {
    return mTicketRepo.findAvailable(destination, date, coachFilter);
}

bool TicketService::completePurchase(const std::string& passport, int ticketId, std::string& outMessage) {
    Ticket* t = mTicketRepo.getById(ticketId);
    if (!t) { outMessage = "Ticket not found"; return false; }

    try {
        t->setStatus(Status::Reserved);
        auto* p = mPassengerRepo.getPassenger(passport);
        if (!p) throw std::runtime_error("Passenger not registered");
        float cost = t->getCost();
        if (p->balance < cost) throw std::runtime_error("Insufficient funds");

        if (!mPassengerRepo.adjustBalance(passport, -cost)) throw std::runtime_error("Balance adjust failed");
        t->setStatus(Status::Sold);
        mPassengerRepo.addPurchasedTicket(passport, ticketId);

        Transaction tx{ ticketId, "purchase", mClock.nowISO(), cost };
        mRegistry.push_back(tx);
        outMessage = "Purchase completed";
        return true;
    }
    catch (const std::exception& ex) {
        t->setStatus(Status::Available); // rollback
        outMessage = std::string("Purchase failed: ") + ex.what();
        return false;
    }
}

float TicketService::calculateRefund(float ticketCost, int daysBeforeTravel) const {
    return round2(mPolicy.calculateRefund(ticketCost, daysBeforeTravel));
}

bool TicketService::completeReturn(const std::string& passport, int ticketId, std::string& outMessage) {
    Ticket* t = mTicketRepo.getById(ticketId);
    if (!t) { outMessage = "Ticket not found"; return false; }
    if (t->getStatus() != Status::Sold) { outMessage = "Ticket is not sold; cannot return"; return false; }

    auto purchased = mPassengerRepo.getPurchasedTickets(passport);
    if (std::find(purchased.begin(), purchased.end(), ticketId) == purchased.end()) { outMessage = "Ticket not owned by passenger"; return false; }

    int days = mClock.daysBetween(t->getDate());

    float refund = calculateRefund(t->getCost(), days);
    if (!mPassengerRepo.adjustBalance(passport, refund)) { outMessage = "Balance refund failed"; return false; }

    t->setStatus(Status::Available);
    mPassengerRepo.removePurchasedTicket(passport, ticketId);

    Transaction tx;
    tx.ticketId = ticketId;
    tx.operation = "return";
    tx.timestamp = mClock.nowISO();
    tx.amount = -refund;
    mRegistry.push_back(tx);

    outMessage = "Return completed; refund: " + std::to_string(refund);
    return true;
}

std::vector<Transaction> TicketService::getRegistry() const { return mRegistry; }

float TicketService::dailyProfit() const {
    float profit = 0.0f;
    for (const auto& tx : mRegistry) {
        if (tx.operation == "purchase") profit += tx.amount;
        else if (tx.operation == "return") profit += tx.amount;
    }
    return round2(profit);
}

void TicketService::saveRegistry(const std::string& path) const {
    std::ofstream out(path);
    if (!out.is_open()) throw RepositoryException("Cannot open file: " + path);
    out << "# Transaction Registry\n";
    for (const auto& tx : mRegistry) {
        out << tx.ticketId << "," << tx.operation << "," << tx.timestamp << "," << std::fixed << std::setprecision(2) << tx.amount << "\n";
    }
}

void TicketService::loadRegistry(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) throw RepositoryException("Cannot open file: " + path);
    mRegistry.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        Transaction tx;
        std::string tmp;
        std::getline(ss, tmp, ','); tx.ticketId = std::stoi(tmp);
        std::getline(ss, tx.operation, ',');
        std::getline(ss, tx.timestamp, ',');
        std::getline(ss, tmp, ','); tx.amount = std::stof(tmp);
        mRegistry.push_back(tx);
    }
}
