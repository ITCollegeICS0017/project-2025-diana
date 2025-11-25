#include "Repositories.h"
#include <algorithm>
#include "Errors.h"
#include <fstream>
#include <sstream>
#include <iomanip>

// TicketRepository
void TicketRepository::addTicket(const Ticket& t) {
    mTickets.push_back(t);
}

std::vector<Ticket> TicketRepository::listAll() const {
    return mTickets;
}

std::vector<int> TicketRepository::findAvailable(const std::string& destination,
    const std::string& date,
    std::optional<Coach> coachFilter) {
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

void TicketRepository::setStoragePath(const std::string& path) { mStoragePath = path; }
std::string TicketRepository::storagePath() const { return mStoragePath; }

// Optional file-backed persistence via adapter (CSV format compatible with FileTicketRepository)
void TicketRepository::save() {
    if (mStoragePath.empty()) return; // no-op when path not configured
    std::ofstream out(mStoragePath);
    if (!out.is_open()) {
        throw RepositoryException(std::string("Cannot open file: ") + mStoragePath);
    }
    out << "# Ticket Repository\n";
    for (const auto& t : mTickets) {
        out << t.toCSV() << "\n";
    }
}

void TicketRepository::load() {
    if (mStoragePath.empty()) return; // no-op when path not configured
    std::ifstream in(mStoragePath);
    if (!in.is_open()) {
        throw RepositoryException(std::string("Cannot open file: ") + mStoragePath);
    }
    mTickets.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string idStr, date, destination, coachStr, costStr, statusStr;
        std::getline(ss, idStr, ',');
        std::getline(ss, date, ',');
        std::getline(ss, destination, ',');
        std::getline(ss, coachStr, ',');
        std::getline(ss, costStr, ',');
        std::getline(ss, statusStr, ',');
        int id = std::stoi(idStr);
        float cost = std::stof(costStr);
        Coach coach = (coachStr == "Sleeper") ? Coach::Sleeper :
            (coachStr == "Compartment") ? Coach::Compartment :
            (coachStr == "Economy") ? Coach::Economy : Coach::FirstClass;
        Status status = (statusStr == "Available") ? Status::Available :
            (statusStr == "Reserved") ? Status::Reserved : Status::Sold;
        mTickets.emplace_back(id, date, cost, destination, coach, status);
    }
}

// TrainRepository (stub)
void TrainRepository::addTrain(const std::string&) {}

// PassengerRepository
void PassengerRepository::addPassenger(const std::string& passport, float balance) {
    if (getPassenger(passport)) return;
    PassengerRecord p{ passport, balance, {} };
    mPassengers.push_back(std::move(p));
}

PassengerRepository::PassengerRecord* PassengerRepository::getPassenger(const std::string& passport) {
    for (auto& p : mPassengers) if (p.passport == passport) return &p;
    return nullptr;
}

const PassengerRepository::PassengerRecord* PassengerRepository::getPassenger(const std::string& passport) const {
    for (const auto& p : mPassengers) if (p.passport == passport) return &p;
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

void PassengerRepository::removePurchasedTicket(const std::string& passport, int ticketId) {
    PassengerRecord* p = getPassenger(passport);
    if (!p) return;
    auto& tickets = p->purchasedTickets;
    tickets.erase(std::remove(tickets.begin(), tickets.end(), ticketId), tickets.end());
}


// --- PassengerRepository persistence ---
void PassengerRepository::save(const std::string& path) const {
    std::ofstream out(path);
    if (!out.is_open()) {
        throw RepositoryException(std::string("Cannot open file: ") + path);
    }
    out << "# Passenger Repository\n";
    for (const auto& p : mPassengers) {
        out << p.passport << "," << std::fixed << std::setprecision(2) << p.balance;
        for (int tid : p.purchasedTickets) {
            out << "," << tid;
        }
        out << "\n";
    }
}

void PassengerRepository::load(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw RepositoryException(std::string("Cannot open file: ") + path);
    }
    mPassengers.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        PassengerRecord rec; rec.purchasedTickets.clear();
        std::string balanceStr;
        std::getline(ss, rec.passport, ',');
        std::getline(ss, balanceStr, ',');
        rec.balance = std::stof(balanceStr);
        std::string tidStr;
        while (std::getline(ss, tidStr, ',')) {
            if (!tidStr.empty()) {
                rec.purchasedTickets.push_back(std::stoi(tidStr));
            }
        }
        mPassengers.push_back(std::move(rec));
    }
}
