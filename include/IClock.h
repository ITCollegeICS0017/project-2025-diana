#pragma once
#include <string>

class IClock {
public:
    virtual ~IClock() = default;
    virtual std::string nowISO() const = 0;
    virtual int daysBetween(const std::string& dateIso) const = 0;
};
