
#pragma once
#include <map>
#include <string>
#include <stdexcept>

class RefundPolicy {
public:
    RefundPolicy() {
        // Default brackets: daysBeforeTravel -> penaltyPercent
        mBrackets = {
            {30, 0.01f},
            {15, 0.05f},
            {3,  0.10f},
            {0,  0.30f},
            {-9999, 1.0f} // Past travel date
        };
    }

    void loadFromFile(const std::string& path) {
        // TODO: Parse JSON or CSV to populate mBrackets dynamically
    }

    float calculateRefund(float ticketCost, int daysBeforeTravel) const {
        for (const auto& [days, penalty] : mBrackets) {
            if (daysBeforeTravel >= days) {
                float refund = ticketCost * (1.0f - penalty);
                return refund;
            }
        }
        throw std::runtime_error("Refund calculation failed");
    }

private:
    std::map<int, float> mBrackets; // Key: daysBeforeTravel, Value: penaltyPercent
};
