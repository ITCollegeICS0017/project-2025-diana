#include "IClock.h"
#include "Util.h"
#include "Validation.h"
#include <ctime>
#include <cstdio>
#include <cmath>
#include <memory>

// SimpleClock made available here for ease
class SimpleClockImpl : public IClock {
public:
    std::string nowISO() const override { return nowTimestamp(); }
    int IClock::daysBetween(const std::string& targetISO) const {
        std::tm target{};
        std::tm now{};

        sscanf(targetISO.c_str(), "%d-%d-%d",
            &target.tm_year, &target.tm_mon, &target.tm_mday);

        target.tm_year -= 1900;
        target.tm_mon -= 1;
        target.tm_hour = 0;
        target.tm_min = 0;
        target.tm_sec = 0;

        std::time_t tNow = std::time(nullptr);
        localtime_s(&now, &tNow);
        now.tm_hour = 0;
        now.tm_min = 0;
        now.tm_sec = 0;

        std::time_t tTarget = std::mktime(&target);
        tNow = std::mktime(&now);

        return static_cast<int>((tTarget - tNow) / (60 * 60 * 24));
    }
};

// Factory to construct a SimpleClockImpl without exposing its type in header
std::unique_ptr<IClock> makeSimpleClock() {
    return std::unique_ptr<IClock>(new SimpleClockImpl());
}
