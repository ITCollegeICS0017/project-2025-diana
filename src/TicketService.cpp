#include "TicketService.h"
#include "Repositories.h"
#include "Util.h"
#include <algorithm>
#include <cmath>

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
    float penaltyPercent = 0.0f;
    if (daysBeforeTravel >= 30) penaltyPercent = 0.01f;
    else if (daysBeforeTravel >= 15) penaltyPercent = 0.05f;
    else if (daysBeforeTravel >= 3) penaltyPercent = 0.10f;
    else if (daysBeforeTravel >= 0) penaltyPercent = 0.30f;
    else penaltyPercent = 1.0f;
    float refund = ticketCost * (1.0f - penaltyPercent);
    return round2(refund);
}

bool TicketService::completeReturn(const std::string& passport, int ticketId, std::string& outMessage) {
    Ticket* t = mTicketRepo.getById(ticketId);
    if (!t) { outMessage = "Ticket not found"; return false; }
    if (t->getStatus() != Status::Sold) { outMessage = "Ticket is not sold; cannot return"; return false; }

    auto purchased = mPassengerRepo.getPurchasedTickets(passport);
    if (std::find(purchased.begin(), purchased.end(), ticketId) == purchased.end()) { outMessage = "Ticket not owned by passenger"; return false; }

    int days = mClock.daysBetween(t->getDate());
    if (days < -1) { outMessage = "Invalid travel date"; return false; }

    float refund = calculateRefund(t->getCost(), days);
    if (!mPassengerRepo.adjustBalance(passport, refund)) { outMessage = "Balance refund failed"; return false; }

    t->setStatus(Status::Available);

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
