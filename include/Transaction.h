#pragma once
#include <string>

struct Transaction {
    int ticketId;
    std::string operation; // "purchase" or "return"
    std::string timestamp;
    float amount; // EUR
};
