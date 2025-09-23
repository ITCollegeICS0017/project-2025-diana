#include <iostream>
#include <string>
using namespace std;

enum Status { Available, Reserved, Sold };

enum Coach { Sleeper, Compartment, Economy, FirstClass };

class Ticket {
    private:
        int mId;
        string mDate;
        float mCost;
        string mDestination;
        string mCoachType;
        string mStatus;
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