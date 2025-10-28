#include "Repositories.h"
#include <algorithm>

// TicketRepository
void TicketRepository::addTicket(const Ticket& t) {
    mTickets.push_back(t);
}

std::vector<Ticket> TicketRepository::listAll() const {
    return mTickets;
}

std::vector<int> TicketRepository::findAvailable(const std::string& destination, const std::string& date, std::optional<Coach> coachFilter) {
    std::vector<int> results;
    for (size_t i = 0; i < mTickets.size(); ++i) {
        const Ticket& t = mTickets[i];
        if (t.getStatus() != Status::Available) continue;
        if (!destination.empty() && t.getDestination() != destination) continue;
        if (!date.empty() && t.getDate() != date) continue;
        if (coachFilter.has_value() && t.getCoachType() != coachFilter.value()) continue;
        results.push_back(static_cast<int>(i));
    }
    return results;
}

Ticket* TicketRepository::getById(int id) {
    for (auto& t : mTickets) {
        if (t.getId() == id) return &t;
    }
    return nullptr;
}

bool TicketRepository::updateStatusById(int id, Status newStatus) {
    Ticket* t = getById(id);
    if (!t) return false;
    t->setStatus(newStatus);
    return true;
}

Ticket* TicketRepository::getByIndex(int idx) {
    if (idx < 0 || static_cast<size_t>(idx) >= mTickets.size()) return nullptr;
    return &mTickets[static_cast<size_t>(idx)];
}

// TrainRepository (stub)
void TrainRepository::addTrain(const std::string&) {}

// PassengerRepository
void PassengerRepository::addPassenger(const std::string& passport, float balance) {
    if (getPassenger(passport)) return;
    PassengerRecord p{passport, balance, {}};
    mPassengers.push_back(std::move(p));
}

PassengerRepository::PassengerRecord* PassengerRepository::getPassenger(const std::string& passport) {
    for (auto& p : mPassengers) if (p.passport == passport) return &p;
    return nullptr;
}

bool PassengerRepository::adjustBalance(const std::string& passport, float delta) {
    PassengerRecord* p = getPassenger(passport);
    if (!p) return false;
    float newBal = p->balance + delta;
    if (newBal < 0.0f) return false;
    p->balance = newBal;
    return true;
}

float PassengerRepository::getBalance(const std::string& passport) const {
    for (const auto& p : mPassengers) if (p.passport == passport) return p.balance;
    return 0.0f;
}

void PassengerRepository::addPurchasedTicket(const std::string& passport, int ticketId) {
    PassengerRecord* p = getPassenger(passport);
    if (!p) return;
    p->purchasedTickets.push_back(ticketId);
}

std::vector<int> PassengerRepository::getPurchasedTickets(const std::string& passport) const {
    for (const auto& p : mPassengers) if (p.passport == passport) return p.purchasedTickets;
    return {};
}
