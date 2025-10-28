#pragma once
#include "Ticket.h"
#include <vector>
#include <optional>
#include <string>

class TicketRepository {
public:
    void addTicket(const Ticket& t);
    std::vector<Ticket> listAll() const;
    std::vector<int> findAvailable(const std::string& destination, const std::string& date, std::optional<Coach> coachFilter = std::nullopt);
    Ticket* getById(int id);
    bool updateStatusById(int id, Status newStatus);
    Ticket* getByIndex(int idx);
private:
    std::vector<Ticket> mTickets;
};

class TrainRepository {
public:
    TrainRepository() = default;
    void addTrain(const std::string& /*trainId*/);
};

class PassengerRepository {
public:
    void addPassenger(const std::string& passport, float balance);
    bool adjustBalance(const std::string& passport, float delta);
    float getBalance(const std::string& passport) const;
    void addPurchasedTicket(const std::string& passport, int ticketId);
    std::vector<int> getPurchasedTickets(const std::string& passport) const;
    struct PassengerRecord {
        std::string passport;
        float balance;
        std::vector<int> purchasedTickets;
    };
    PassengerRecord* getPassenger(const std::string& passport);
private:
    std::vector<PassengerRecord> mPassengers;
};
