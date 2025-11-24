
#include "FileTicketRepository.h"
#include "Errors.h"
#include "Types.h"
#include <sstream>

void FileTicketRepository::load() {
    std::ifstream in(mFilePath);
    if (!in.is_open()) {
        throw RepositoryException("Cannot open file: " + mFilePath);
    }

    mTickets.clear();
    std::string line;

    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);

        std::string idStr, date, destination, coachStr, costStr, statusStr;
        std::getline(ss, idStr, ',');
        std::getline(ss, date, ',');
        std::getline(ss, destination, ',');
        std::getline(ss, coachStr, ',');
        std::getline(ss, costStr, ',');
        std::getline(ss, statusStr, ',');

        int id = std::stoi(idStr);
        float cost = std::stof(costStr);

        Coach coach =
            (coachStr == "Sleeper") ? Coach::Sleeper :
            (coachStr == "Compartment") ? Coach::Compartment :
            (coachStr == "Economy") ? Coach::Economy :
            Coach::FirstClass;

        Status status =
            (statusStr == "Available") ? Status::Available :
            (statusStr == "Reserved") ? Status::Reserved :
            Status::Sold;

        mTickets.emplace_back(id, date, cost, destination, coach, status);
    }
}
