#include "Ticket.h"
#include "Types.h"
#include <iostream>

Ticket::Ticket(int id,
               const std::string& date,
               float cost,
               const std::string& destination,
               Coach coachType,
               Status status)
    : mId(id),
      mDate(date),
      mCost(cost),
      mDestination(destination),
      mCoachType(coachType),
      mStatus(status)
{
    if (id <= 0) throw std::invalid_argument("Ticket id must be positive");
    if (date.empty()) throw std::invalid_argument("Date required");
    if (cost < 0.0f) throw std::invalid_argument("Cost cannot be negative");
}

void Ticket::showDetails() const {
    std::cout << "Ticket ID: " << mId
              << ", Destination: " << mDestination
              << ", Date: " << mDate
              << ", Cost: " << std::fixed << std::setprecision(2) << mCost
              << ", Coach: " << CoachToString(mCoachType)
              << ", Status: " << StatusToString(mStatus) << std::endl;
}

int Ticket::getId() const { return mId; }
std::string Ticket::getDate() const { return mDate; }
float Ticket::getCost() const { return mCost; }
std::string Ticket::getDestination() const { return mDestination; }
Coach Ticket::getCoachType() const { return mCoachType; }
Status Ticket::getStatus() const { return mStatus; }
void Ticket::setStatus(Status s) { mStatus = s; }

std::string Ticket::toCSV() const {
    std::ostringstream oss;
    oss << mId << "," << mDate << "," << mDestination << "," << CoachToString(mCoachType) << "," << std::fixed << std::setprecision(2) << mCost << "," << StatusToString(mStatus);
    return oss.str();
}
