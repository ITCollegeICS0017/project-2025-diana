#include <iostream>
#include <string>
#include <vector>
using namespace std;


enum Status { Available, Reserved, Sold };

enum Coach { Sleeper, Compartment, Economy, FirstClass };

struct Transaction {
    int ticketId;
    string operation; // "purchase", "return"
    string timestamp;
    float amount; // Assumed in EUR
};

class Ticket {
    private:
        int mId;
        string mDate;
        float mCost; // Assumed in EUR
        string mDestination;
        string mCoachType;
        Status mStatus;
    public:
        Ticket(int id, string date, float cost, string destination, string coachType, Status status) {
            mId = id;
            mDate = date;
            mCost = cost;
            mDestination = destination;
            mCoachType = coachType;
            mStatus = status;
        }
    
        void showDetails() {
        cout << "Ticket ID: " << mId 
             << ", Destination: " << mDestination
             << ", Date: " << mDate 
             << ", Cost: " << mCost
             << ", Status: " << mStatus << endl;
        }

        void setStatus(Status s) { mStatus = s; }

        Status getStatus() { return mStatus; }
        int getId() { return mId; }
        string getDestination() { return mDestination; }
        string getDate() { return mDate; }
        float getCost() { return mCost; }
};

class Database {
    private:
        vector<Ticket> tickets;
    public:
        void addTicket(const Ticket& t) {
            tickets.push_back(t);
            cout << "Ticket added to database.\n";
        }


        void listTickets() {
            cout << "Listing tickets:\n";
            for (auto& t : tickets) {
                t.showDetails();
            }
        }

        // Ticket search, optional coach type filter not implemented yet
        Ticket* findAvailableTicket(string destination, string date) {
            for (auto& t : tickets) {
                if (t.getDestination() == destination && t.getDate() == date && t.getStatus() == Available) {
                    cout << "Available ticket found (ID: " << t.getId() << ").\n";
                    return &t;
                }
            }
            cout << "No available ticket found for " << destination << " on " << date << "\n";
            return nullptr;
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
            cout << "Client requests ticket return.\n";
        }

        void requestPurchase() {
            cout << "Client requests ticket purchase.\n";
        }

        void listTickets() {
            cout << "Client has " << purchasedTicketIds.size() << " purchased tickets.\n";
        }
};

class Cashier {

};



int main() {

}