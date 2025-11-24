
#pragma once
#include "IRepository.h"
#include "Ticket.h"
#include <fstream>
#include <string>

class FileTicketRepository : public IRepository<Ticket> {
public:
    explicit FileTicketRepository(const std::string& filePath) : mFilePath(filePath) {}

    void add(const Ticket& t) override { mTickets.push_back(t); }
    std::vector<Ticket> listAll() const override { return mTickets; }

    void save() override {
        std::ofstream out(mFilePath);
        out << "# Ticket Repository\n";
        for (const auto& t : mTickets) out << t.toCSV() << "\n";
    }

    void load() override {
        // TODO: parse CSV and populate mTickets
    }

private:
    std::string mFilePath;
    std::vector<Ticket> mTickets;
};
