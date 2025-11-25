
#pragma once
#include <string>
#include <iostream>
#include <functional>

class Monitoring {
public:
    static void checkBalance(float balance, float threshold = 10.0f) {
        if (balance < threshold) {
            emitAlert("LOW_BALANCE", "Balance below threshold: " + std::to_string(balance));
        }
    }

    static void checkTicketAvailability(int availableCount, int threshold = 5) {
        if (availableCount < threshold) {
            emitAlert("LOW_TICKET_AVAILABILITY", "Tickets below threshold: " + std::to_string(availableCount));
        }
    }

    static void emitAlert(const std::string& code, const std::string& message) {
        // For now, just log to console; later integrate with event bus or monitoring system
        std::cout << "[ALERT] Code: " << code << " | " << message << std::endl;
        // TODO: Write to structured log or send to external monitoring
    }
};
