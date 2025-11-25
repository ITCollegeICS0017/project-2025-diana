#include "Types.h"

std::string StatusToString(Status s) {
    switch (s) {
        case Status::Available: return "Available";
        case Status::Reserved:  return "Reserved";
        case Status::Sold:      return "Sold";
        default:                return "Unknown";
    }
}

std::string CoachToString(Coach c) {
    switch (c) {
        case Coach::Sleeper:    return "Sleeper";
        case Coach::Compartment: return "Compartment";
        case Coach::Economy:    return "Economy";
        case Coach::FirstClass: return "FirstClass";
        default:                return "Unknown";
    }
}