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
    int daysBetween(const std::string& dateIso) const override {
        std::tm tmTarget = {};
        if (std::sscanf(dateIso.c_str(), "%d-%d-%d", &tmTarget.tm_year, &tmTarget.tm_mon, &tmTarget.tm_mday) != 3) {
            return -1;
        }
        tmTarget.tm_year -= 1900;
        tmTarget.tm_mon -= 1;
        tmTarget.tm_hour = 0; tmTarget.tm_min = 0; tmTarget.tm_sec = 0;

        std::time_t tTarget = std::mktime(&tmTarget);
        if (tTarget == -1) return -1;

        std::time_t tNow = std::time(nullptr);
        double days = std::difftime(tTarget, tNow) / (60.0 * 60.0 * 24.0);
        int result = static_cast<int>(std::floor(days + 0.5));
        return (result < 0) ? 0 : result;
    }
};

// Factory to construct a SimpleClockImpl without exposing its type in header
std::unique_ptr<IClock> makeSimpleClock() {
    return std::unique_ptr<IClock>(new SimpleClockImpl());
}
