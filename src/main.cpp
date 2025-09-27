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
        Coach mCoachType;
        Status mStatus;
    public:
        Ticket(int id, string date, float cost, string destination, Coach coachType, Status status) {
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
        vector<Ticket> mTickets;
    public:
        void addTicket(const Ticket& t) {
            mTickets.push_back(t);
            cout << "Ticket added to database.\n";
        }


        void listTickets() {
            cout << "Listing tickets:\n";
            for (auto& t : mTickets) {
                t.showDetails();
            }
        }

        // Ticket search, optional coach type filter, not implemented yet
        void findAvailableTicket(string destination, string date) {
            cout << "Ticket found according to filters.\n";
        }

        // Ticket status change, not implemented yet
        void updateTicketStatus(int id, Status newStatus) {
            cout << "Ticket status changed.\n";
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