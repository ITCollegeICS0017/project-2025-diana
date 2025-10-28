#pragma once
#include <string>
#include <memory>

class IClock {
public:
    virtual ~IClock() = default;
    virtual std::string nowISO() const = 0;
    virtual int daysBetween(const std::string& dateIso) const = 0;
};

std::unique_ptr<IClock> makeSimpleClock();