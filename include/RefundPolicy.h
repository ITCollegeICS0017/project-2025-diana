
#pragma once
#include "RefundPolicy.h"
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>


class RefundPolicy {
public:
    RefundPolicy();
    void loadFromFile(const std::string& path);
    float calculateRefund(float ticketCost, int daysBeforeTravel) const;
private:
    std::map<int, float> mBrackets;
};

