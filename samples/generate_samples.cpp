#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <cstdlib>

// -------------------------------------------------------------
// Helpers
// -------------------------------------------------------------

std::string formatDate(const std::tm& tm) {
    char buf[11];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm);
    return buf;
}

std::tm addDays(std::tm base, int days) {
    base.tm_mday += days;
    std::mktime(&base); // normalize
    return base;
}

// -------------------------------------------------------------
// Ticket generation
// -------------------------------------------------------------

void generateTickets(const std::tm& today) {
    std::ofstream out("tickets.csv");
    if (!out) {
        std::cerr << "Failed to open tickets.csv for writing\n";
        return;
    }

    int id = 1;

    auto writeTicket =
        [&](int daysFromToday,
            double price,
            const std::string& dest,
            const std::string& coach,
            const std::string& status)
        {
            std::tm date = addDays(today, daysFromToday);
            out << id++ << ","
                << formatDate(date) << ","
                << dest << ","
                << coach << ","
                << std::fixed << std::setprecision(2) << price << ","
                << status << "\n";
        };

    // ===== >= 30 days (1%) =====
    writeTicket(40, 55.00, "Tallinn", "Economy", "Available");
    writeTicket(45, 90.00, "Vilnius", "FirstClass", "Available");

    // ===== >= 15 days (5%) =====
    writeTicket(20, 42.50, "Riga", "Compartment", "Available");
    writeTicket(18, 68.00, "Warsaw", "Economy", "Available");

    // ===== >= 3 days (10%) =====
    writeTicket(5, 35.00, "Helsinki", "Economy", "Available");
    writeTicket(3, 72.00, "Tallinn", "FirstClass", "Available");

    // ===== 1 day before (30%) =====
    writeTicket(1, 25.00, "Vilnius", "Economy", "Available");

    // ===== Day of travel (30%) =====
    writeTicket(0, 120.00, "Warsaw", "FirstClass", "Available");

    // ===== Past departure (100%) =====
    writeTicket(-2, 50.00, "Riga", "Economy", "Available");

    out.close();

    std::cout << "tickets.csv generated (today = "
        << formatDate(today) << ")\n";
}

// -------------------------------------------------------------
// Refund policy generation
// -------------------------------------------------------------

void generateRefundPolicy() {
    std::ofstream out("refund_policy.csv");
    if (!out) {
        std::cerr << "Failed to open refund_policy.csv for writing\n";
        return;
    }

    out << "30,0.01\n";
    out << "15,0.05\n";
    out << "3,0.10\n";
    out << "0,0.30\n";
    out << "-9999,1.00\n";

    out.close();
    std::cout << "refund_policy.csv generated\n";
}

// -------------------------------------------------------------
// main
// -------------------------------------------------------------

int main(int argc, char* argv[]) {
    std::tm today{};
    today.tm_hour = today.tm_min = today.tm_sec = 0;

    if (argc == 2) {
        // Parse explicit date YYYY-MM-DD
        std::istringstream ss(argv[1]);
        ss >> std::get_time(&today, "%Y-%m-%d");
        if (ss.fail()) {
            std::cerr << "Invalid date format. Use YYYY-MM-DD\n";
            return 1;
        }
    }
    else {
        // Use system local date (MSVC-safe)
        std::time_t now = std::time(nullptr);
        localtime_s(&today, &now);
        today.tm_hour = today.tm_min = today.tm_sec = 0;
    }

    std::cout << "Generating sample data...\n";

    generateTickets(today);
    generateRefundPolicy();

    std::cout << "Sample data generation complete.\n";
    return 0;
}
