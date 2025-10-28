#include "Util.h"
#include <ctime>
#include <cmath>
#include <sstream>
#include <iomanip>

std::string nowTimestamp() {
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

float round2(float v) {
    return std::floor((v + 0.005f) * 100.0f) / 100.0f;
}
