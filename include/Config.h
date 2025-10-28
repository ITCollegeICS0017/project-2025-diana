#pragma once
#include <string>

namespace Config {
    constexpr int MAX_MEMORY_MB = 100;
    constexpr int MAX_DB_FILE_MB = 50;
    constexpr int OP_TIMEOUT_SECONDS = 3;
    constexpr float EPS_ROUND = 0.005f;
    inline const std::string APP_NAME = "Railway Ticket Sales";
}
