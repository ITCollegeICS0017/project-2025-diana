
#include "RefundPolicy.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

void RefundPolicy::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open refund policy file: " + path);
    }

    // Clear and reload brackets (CSV format: days,penaltyPercent)
    mBrackets.clear();

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string daysStr, penaltyStr;
        std::getline(ss, daysStr, ',');
        std::getline(ss, penaltyStr, ',');

        const int   days = std::stoi(daysStr);
        const float penalty = std::stof(penaltyStr);

        mBrackets[days] = penalty;
    }
}

float RefundPolicy::calculateRefund(float ticketCost, int daysBeforeTravel) const {
    // Iterate in map order (ascending keys). We want the first bracket where daysBeforeTravel >= key.
    // If you store brackets like {30, 0.01}, {15, 0.05}, {3, 0.10}, {0, 0.30}, {-9999, 1.0},
    // this logic matches your original policy behavior.
    for (const auto& kv : mBrackets) {
        const int   days = kv.first;
        const float penalty = kv.second;
        if (daysBeforeTravel >= days) {
            return ticketCost * (1.0f - penalty);
        }
    }
    throw std::runtime_error("Refund calculation failed");
}
