#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;

/* ---------- Configuration / Enums ---------- */

enum Status { Available, Reserved, Sold };

enum Coach { Sleeper, Compartment, Economy, FirstClass };

struct Transaction {
    int ticketId;
    string operation; // "purchase", "return"
    string timestamp;
    float amount; // Assumed in EUR
};

namespace Config {
    constexpr int MAX_MEMORY_MB = 100;
    constexpr int MAX_DB_FILE_MB = 50;
    constexpr int OP_TIMEOUT_SECONDS = 3;
    constexpr float EPS_ROUND = 0.005; // used for rounding to 2 decimals
}

/* ---------- Utility helpers ---------- */

static string StatusToString(Status s) {
    switch (s) {
        case Status::Available: return "Available";
        case Status::Reserved:  return "Reserved";
        case Status::Sold:      return "Sold";
        default: return "Unknown";
    }
}

static string CoachToString(Coach c) {
    switch (c) {
        case Coach::Sleeper: return "Sleeper";
        case Coach::Compartment: return "Compartment";
        case Coach::Economy: return "Economy";
        case Coach::FirstClass: return "FirstClass";
        default: return "Unknown";
    }
}

static float round2(float v) {
    // simple rounding to two decimals
    return static_cast<float>(std::floor((v + Config::EPS_ROUND) * 100.0f) / 100.0f);
}

static string nowTimestamp() {
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", std::localtime(&t));
    return string(buf);
}

/* ---------- Data models ---------- */

class Ticket {
    private:
        int mId;
        string mDate;
        float mCost; // Assumed in EUR
        string mDestination;
        Coach mCoachType;
        Status mStatus;
    public:
        Ticket(int id,
            const string& date,
            float cost,
            const string& destination,
            Coach coachType,
            Status status)
            : mId(id),
            mDate(date),
            mCost(cost),
            mDestination(destination),
            mCoachType(coachType),
            mStatus(status)
        {
            // Runtime checks
            if (id <= 0) throw std::invalid_argument("Ticket id must be positive");
            if (date.empty()) throw std::invalid_argument("Date is required");
            if (cost < 0.0f) throw std::invalid_argument("Cost cannot be negative");
        }
    
        void showDetails() {
        cout << "Ticket ID: " << mId 
             << ", Destination: " << mDestination
             << ", Date: " << mDate 
             << ", Cost: " << fixed << setprecision(2) mCost
             << ", Coach: " << CoachToString(mCoachType)
             << ", Status: " << StatusToString(mStatus) << endl;
        }

        void setStatus(Status s) { mStatus = s; }

        Status getStatus() { return mStatus; }
        int getId() { return mId; }
        string getDestination() { return mDestination; }
        string getDate() { return mDate; }
        float getCost() { return mCost; }
        Coach getCoachType() const { return mCoachType; }

        string toCSV() const {
            std::ostringstream oss;
            oss << mId << "," << mDate << "," << mDestination << "," << CoachToString(mCoachType) << "," << fixed << setprecision(2) << mCost << "," << StatusToString(mStatus);
            return oss.str();
        }
};



/* ---------- IClock interface and implementation ---------- */

class IClock {
public:
    virtual ~IClock() = default;
    virtual string nowISO() const = 0;
    virtual int daysBetween(const string& dateIso) const = 0; // simple helper: days from now to given date (non-negative if in future)
};

class SimpleClock : public IClock {
public:
    string nowISO() const override {
        return nowTimestamp();
    }

    int daysBetween(const string& dateIso) const override {
        // Very small helper to compute rough difference in days between now and a YYYY-MM-DD string
        // If parsing fails, return -1
        std::tm tmTarget = {};
        if (sscanf(dateIso.c_str(), "%d-%d-%d", &tmTarget.tm_year, &tmTarget.tm_mon, &tmTarget.tm_mday) != 3) {
            return -1;
        }
        tmTarget.tm_year -= 1900;
        tmTarget.tm_mon -= 1;
        tmTarget.tm_hour = 0; tmTarget.tm_min = 0; tmTarget.tm_sec = 0;
        std::time_t tTarget = std::mktime(&tmTarget);
        if (tTarget == -1) return -1;
        std::time_t tNow = std::time(nullptr);
        double days = std::difftime(tTarget, tNow) / (60.0 * 60.0 * 24.0);
        return static_cast<int>(std::floor(days + 0.5)); // round to nearest day
    }
};

/* ---------- Repositories ---------- */

class TicketRepository {
private:
    vector<Ticket> mTickets;
public:
    void addTicket(const Ticket& t) {
        mTickets.push_back(t);
    }

    vector<Ticket> listAll() const {
        return mTickets;
    }

    // Search available tickets with optional coach filter
    vector<int> findAvailable(const string& destination, const string& date, optional<Coach> coachFilter = nullopt) {
        vector<int> results;
        for (size_t i = 0; i < mTickets.size(); ++i) {
            const Ticket& t = mTickets[i];
            if (t.getStatus() != Status::Available) continue;
            if (!destination.empty() && t.getDestination() != destination) continue;
            if (!date.empty() && t.getDate() != date) continue;
            if (coachFilter.has_value() && t.getCoachType() != coachFilter.value()) continue;
            results.push_back(static_cast<int>(i)); // index in repository
        }
        return results;
    }

    Ticket* getById(int id) {
        for (auto& t : mTickets) {
            if (t.getId() == id) return &t;
        }
        return nullptr;
    }

    bool updateStatusById(int id, Status newStatus) {
        Ticket* t = getById(id);
        if (!t) return false;
        t->setStatus(newStatus);
        return true;
    }

    // convenience to fetch by index
    Ticket* getByIndex(int idx) {
        if (idx < 0 || static_cast<size_t>(idx) >= mTickets.size()) return nullptr;
        return &mTickets[static_cast<size_t>(idx)];
    }
};

class Client {
    private:
        string mPassportData;
        float mBalance;
        vector<int> purchasedTicketIds;

    public:
        // Constructor
        Client(string passport, float balance) : mPassportData(passport), mBalance(balance) {}

        void requestReturn() {
            // Triggers Cashier.completeReturn() using the ticket ID, not implemented yet
            cout << "Client requests ticket return.\n";
        }

        void requestPurchase() {
            // Triggers Cashier.completePurchase() using the ticket ID, not implemented yet
            cout << "Client requests ticket purchase.\n";
        }

        void listTickets() {
            cout << "Client has " << purchasedTicketIds.size() << " purchased tickets.\n";
        }
};

class Cashier {
    private:
        vector<Transaction> mRegistry;
    public:
        void completePurchase() {
            // Will update ticket status to "Reserved", then take client's money, then update ticket status to "Sold", then print a check, then update mRegistry with operation details, not implemented yet
            cout << "Cashier completes purchase.\n";
        }
        
        void completeReturn() {
            // Will calculate return, then apply to the client, then update ticket status to "Available", then print a check, then update mRegistry with operation details, not implemented yet
            cout << "Cashier completes return.\n";
        }
        
        void submitReport() {
            // Will loop through the registry and save the log
            cout << "Cashier submits daily report.\n";
        }
};



int main() {
    Ticket t1(1, "2025-10-01", 50.0, "Tallinn", Sleeper, Available);
    Ticket t2(2, "2025-10-02", 30.0, "Riga", Compartment, Available);


    Database db;
    db.addTicket(t1);
    db.addTicket(t2);
    db.listTickets();
    Cashier testCashier;

    db.findAvailableTicket("Tallinn", "2025-10-01");

    Client testClient("John", 100.0);
    testClient.requestPurchase();
    testCashier.completePurchase();
    db.updateTicketStatus(1, Reserved);

    testCashier.submitReport();


    return 0;
}