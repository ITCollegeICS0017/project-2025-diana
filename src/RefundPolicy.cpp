
#include "RefundPolicy.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

RefundPolicy::RefundPolicy() {
    mBrackets = {
        {30, 0.01f},
        {15, 0.05f},
        {3, 0.10f},
        {0, 0.30f},
        {-9999, 1.0f}
    };
}

void RefundPolicy::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open refund policy file: " + path);
    }
    mBrackets.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string daysStr, penaltyStr;
        std::getline(ss, daysStr, ',');
        std::getline(ss, penaltyStr, ',');
        int days = std::stoi(daysStr);
        float penalty = std::stof(penaltyStr);
        mBrackets[days] = penalty;
    }
}

float RefundPolicy::calculateRefund(float ticketCost, int daysBeforeTravel) const {
    for (const auto& kv : mBrackets) {
        if (daysBeforeTravel >= kv.first) {
            return ticketCost * (1.0f - kv.second);
        }
    }
    throw std::runtime_error("Refund calculation failed");
}
