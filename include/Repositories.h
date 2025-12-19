
#pragma once
#include "Ticket.h"
#include "Types.h"
#include "IRepository.h"
#include <vector>
#include <optional>
#include <string>

class TicketRepository : public IRepository<Ticket> {
public:
    // IRepository<Ticket>
    void add(const Ticket& t) override { addTicket(t); }
    std::vector<Ticket> listAll() const override;
    void save() override;     // optional file-backed persistence
    void load() override;     // optional file-backed persistence

    // Existing API preserved
    void addTicket(const Ticket& t);
    std::vector<int> findAvailable(const std::string& destination,
                                   const std::string& date,
                                   std::optional<Coach> coachFilter = std::nullopt);
    Ticket* getById(int id);
    bool updateStatusById(int id, Status newStatus);
    Ticket* getByIndex(int idx);

    // Adapter configuration for persistence
    void setStoragePath(const std::string& path);
    std::string storagePath() const;

private:
    std::vector<Ticket> mTickets;
    std::string mStoragePath; // empty => no-op save/load
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
    void removePurchasedTicket(const std::string& passport, int ticketId); // ✅ NEW
    std::vector<int> getPurchasedTickets(const std::string& passport) const;

    struct PassengerRecord {
        std::string passport;
        float balance;
        std::vector<int> purchasedTickets;
    };

    PassengerRecord* getPassenger(const std::string& passport);
    const PassengerRecord* getPassenger(const std::string& passport) const;
    void save(const std::string& path) const;
    void load(const std::string& path);

private:
    std::vector<PassengerRecord> mPassengers;
};

