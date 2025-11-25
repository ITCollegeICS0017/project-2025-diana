
#pragma once
#include <string>
#include <regex>

struct ValidationResult {
    bool ok;
    std::string errorCode;
    std::string message;
};

class Validator {
public:
    static ValidationResult validateAmount(float amount) {
        if (amount <= 0.0f) return {false, "ERR_AMOUNT", "Amount must be greater than zero"};
        return {true, "", ""};
    }

    static ValidationResult validateISODate(const std::string& date) {
        std::regex isoPattern(R"(^\d{4}-\d{2}-\d{2}$)");
        if (!std::regex_match(date, isoPattern)) return {false, "ERR_DATE", "Invalid ISO date format"};
        return {true, "", ""};
    }

    static ValidationResult validatePassport(const std::string& passport) {
        if (passport.size() < 4 || passport.find(' ') != std::string::npos)
            return {false, "ERR_PASSPORT", "Passport must be alphanumeric and at least 4 characters"};
        return {true, "", ""};
    }

    static ValidationResult validateTicketId(int id) {
        if (id <= 0) return {false, "ERR_TICKET_ID", "Ticket ID must be positive"};
        return {true, "", ""};
    }

    static ValidationResult validateCoach(const std::string& coach) {
        static const std::string valid[] = {"Sleeper", "Compartment", "Economy", "FirstClass"};
        for (auto& v : valid) if (coach == v) return {true, "", ""};
        return {false, "ERR_COACH", "Unsupported coach type"};
    }
};
