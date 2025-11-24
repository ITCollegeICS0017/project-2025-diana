#pragma once
#include "Repositories.h"
#include "Transaction.h"
#include "IClock.h"
#include "RefundPolicy.h"
#include <vector>
#include <string>
#include <optional>


class TicketService {
public:
    TicketService(TicketRepository& tr, PassengerRepository& pr, const IClock& clock);
    std::vector<int> searchAvailable(const std::string& destination, const std::string& date, std::optional<Coach> coachFilter = std::nullopt);
    bool completePurchase(const std::string& passport, int ticketId, std::string& outMessage);
    bool completeReturn(const std::string& passport, int ticketId, std::string& outMessage);
    float calculateRefund(float ticketCost, int daysBeforeTravel) const;
    std::vector<Transaction> getRegistry() const;
    float dailyProfit() const;

    // NEW persistence methods
    void saveRegistry(const std::string& path) const;
    void loadRegistry(const std::string& path);

private:
    TicketRepository& mTicketRepo;
    PassengerRepository& mPassengerRepo;
    std::vector<Transaction> mRegistry;
    const IClock& mClock;

    RefundPolicy mPolicy;
};

