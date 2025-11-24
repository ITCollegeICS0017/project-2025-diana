
#pragma once
#include "IRepository.h"
#include "Ticket.h"
#include "Errors.h"
#include <fstream>
#include <string>
#include <vector>

class FileTicketRepository : public IRepository<Ticket> {
public:
    explicit FileTicketRepository(const std::string& filePath) : mFilePath(filePath) {}

    void add(const Ticket& t) override { mTickets.push_back(t); }
    std::vector<Ticket> listAll() const override { return mTickets; }

    void save() override {
        std::ofstream out(mFilePath);
        if (!out.is_open()) throw RepositoryException("Cannot open file: " + mFilePath);
        out << "# Ticket Repository\n";
        for (const auto& t : mTickets) out << t.toCSV() << "\n";
    }

    void load() override;  // Implemented in .cpp

private:
    std::string mFilePath;
    std::vector<Ticket> mTickets;
};
