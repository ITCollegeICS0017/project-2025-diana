#pragma once
#include <string>

enum class Status { Available, Reserved, Sold };
enum class Coach { Sleeper, Compartment, Economy, FirstClass };

std::string StatusToString(Status s);
std::string CoachToString(Coach c);
