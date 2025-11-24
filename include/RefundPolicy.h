
#pragma once
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

class RefundPolicy {
public:
    RefundPolicy() {
        // Default brackets
        mBrackets = {
            {30, 0.01f},
            {15, 0.05f},
            {3, 0.10f},
            {0, 0.30f},
            {-9999, 1.0f}
        };
    }

    void loadFromFile(const std::string& path); // Implemented in .cpp
    float calculateRefund(float ticketCost, int daysBeforeTravel) const;

private:
    std::map<int, float> mBrackets;
};
