#include <iostream>
#include <string>
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
}

class Database {
    
}

class Client {
    public:
        void requestReturn() {

        }

    private:
        string mPassportData;
        float mBalance;
        
}

class Cashier {

}



int main() {

}