#pragma once
#include "Types.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

class Ticket {
public:
    Ticket(int id,
           const std::string& date,
           float cost,
           const std::string& destination,
           Coach coachType,
           Status status);

    void showDetails() const;

    // getters
    int getId() const;
    std::string getDate() const;
    float getCost() const;
    std::string getDestination() const;
    Coach getCoachType() const;
    Status getStatus() const;

    // setters
    void setStatus(Status s);

    // serialization
    std::string toCSV() const;

private:
    int mId;
    std::string mDate;
    float mCost;
    std::string mDestination;
    Coach mCoachType;
    Status mStatus;
};
