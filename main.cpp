#include <iostream>
#include <queue>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class Customer {
private:
int customerID;
int arrivalTime;
int serviceTime;
int exitTime;
int waitingTime;

public:
      Customer() {}


      Customer(int ID, int arrival, int service)
      {
        this->customerID = ID;
        this->arrivalTime = arrival;
        this->serviceTime = service;
        this->exitTime = 0;
        this->waitingTime = 0;
      }

int getCustomerID()
 {
    return customerID;
 }

int getArrivalTime()
 {
    return arrivalTime;
}

int getServiceTime()
 {
    return serviceTime;
}

int getExitTime() const {
    return exitTime;
}

void setExitTime(int exit) {
    exitTime = exit;
}

int getWaitingTime()  {
    return waitingTime;
}

void setWaitingTime(int wait) {
    waitingTime = wait;
}


int calculateExitTime()  {
    return arrivalTime + waitingTime + serviceTime;
}

bool operator<(const Customer& other) const
{
    if (this->arrivalTime == other.arrivalTime) {
        return this->customerID < other.customerID;
    } else {
        return this->arrivalTime > other.arrivalTime;
    }
}
};

class Teller {
private:
int tellerID;
int totalCustomersServed;
int totalServiceTime;
bool isFree;
Customer* currentCustomer;
int finishingTime;

public:
Teller() {}


Teller(int ID) : tellerID(ID), totalCustomersServed(0), totalServiceTime(0), isFree(true), currentCustomer(nullptr), finishingTime(0) {}

int getTellerID()  {
    return tellerID;
}

void setTellerID(int ID) {
    tellerID = ID;
}

int getTotalCustomersServed()  {
    return totalCustomersServed;
}

void setTotalCustomersServed(int served) {
    totalCustomersServed = served;
}

int getTotalServiceTime()  {
    return totalServiceTime;
}

void setTotalServiceTime(int serviceTime) {
    totalServiceTime = serviceTime;
}

bool checkAvailability()  {
    return isFree;
}

void setIsFree(bool free) {
    isFree = free;
}

Customer* getCurrentCustomer()  {
    return currentCustomer;
}

void setCurrentCustomer(Customer* customer) {
    currentCustomer = customer;
}

int getFinishingTime() const {
    return finishingTime;
}

void setFinishingTime(int time) {
    finishingTime = time;
}
};

class Bank {
private:
vector<Teller*> tellers;
queue<Customer*> customerQueue;
priority_queue<Customer*> priorityCustomerQueue;
int totalWaitingTime;
int totalServiceTime;
int totalCustomersServed;

public:
Bank(int numTellers) : totalWaitingTime(0), totalServiceTime(0), totalCustomersServed(0)
{
  for (int i = 1; i <= numTellers; i++)
     {
        tellers.push_back(new Teller(i));
     }
}


~Bank() {
    for (Teller* teller : tellers) {
        delete teller;
    }

    while (!customerQueue.empty()) {
        delete customerQueue.front();
        customerQueue.pop();
    }

    while (!priorityCustomerQueue.empty()) {
        delete priorityCustomerQueue.top();
        priorityCustomerQueue.pop();
    }
}

void addCustomer(Customer* customer) {
    if (customer->getCustomerID() % 3 == 0) //this will be the priority queue
    {
        priorityCustomerQueue.push(customer);

    } else {
        customerQueue.push(customer);
    }
}

void enqueueCustomer(Customer* customer) {
    if (customer->getCustomerID() % 3 == 0) {
        priorityCustomerQueue.push(customer);
    } else {
        customerQueue.push(customer);
    }
}

void dequeueCustomer()
{
    if (!priorityCustomerQueue.empty())
        {
        delete priorityCustomerQueue.top();
        priorityCustomerQueue.pop();
        }else if (!customerQueue.empty()) {
        delete customerQueue.front();
        customerQueue.pop();
    }
}

void displayQueueStatus() {
    cout << "Customer Queue Status: " << endl;
    cout << "Priority Queue: ";


    priority_queue<Customer*> tempPriorityQueue = priorityCustomerQueue;
    while (!tempPriorityQueue.empty())
    {
        cout << tempPriorityQueue.top()->getCustomerID() << " ";
         tempPriorityQueue.pop();
    }
cout << endl;

    cout << "Normal Queue: ";
    queue<Customer*> tempQueue = customerQueue;
    while (!tempQueue.empty()) {
        cout << tempQueue.front()->getCustomerID() << " ";
        tempQueue.pop();
    }
 cout << endl;
}

void serveCustomers()
{
    int currentTime = 0;
    while (!priorityCustomerQueue.empty() || !customerQueue.empty() || !allTellersFree())
        {
        updateTellersAvailability(currentTime);

        for (Teller* teller : tellers) {
            if (teller->checkAvailability())
                {
                  if (!priorityCustomerQueue.empty()) {
                    Customer* customer = priorityCustomerQueue.top();
                    priorityCustomerQueue.pop();

                    processCustomer(teller, customer, currentTime);
                } else if (!customerQueue.empty())
                {
                    Customer* customer = customerQueue.front();
                    customerQueue.pop();

                    processCustomer(teller, customer, currentTime);
            }
            }
    }

        currentTime++;
    }
}

void processCustomer(Teller* teller, Customer* customer, int currentTime)
{
    teller->setIsFree(false);
    teller->setCurrentCustomer(customer);

    int serviceTime = customer->getServiceTime();
    int finishingTime = currentTime + serviceTime;

    customer->setWaitingTime(abs(currentTime - customer->getArrivalTime()));
    customer->setExitTime(finishingTime);

    teller->setTotalCustomersServed(teller->getTotalCustomersServed() + 1);
    teller->setTotalServiceTime(teller->getTotalServiceTime() + serviceTime);
    totalCustomersServed++;
    totalServiceTime += serviceTime;
    totalWaitingTime += customer->getWaitingTime();

    teller->setFinishingTime(finishingTime);
}

void updateTellersAvailability(int currentTime) {
    for (Teller* teller : tellers) {
        if (!teller->checkAvailability() && teller->getFinishingTime() == currentTime) {
            teller->setIsFree(true);
            teller->setCurrentCustomer(nullptr);
        }
    }
}

bool allTellersFree() const {
    for (Teller* teller : tellers) {
        if (!teller->checkAvailability()) {
            return false;
        }
    }
    return true;
}

void displayStatistics() {
    cout << "Bank Statistics:" << endl;
    cout << "Total Customers Served: " << totalCustomersServed << endl;
    cout << "Total Service Time: " << abs(totalServiceTime) << " minutes" << endl;
    cout << "Average Service Time per Customer: " << static_cast<double>(abs(totalServiceTime)) / totalCustomersServed << " minutes" << endl;
    cout << "Total Waiting Time: " << abs(totalWaitingTime) << " minutes" << endl;
    cout << "Average Waiting Time per Customer: " << static_cast<double>(abs(totalWaitingTime)) / totalCustomersServed << " minutes" << endl;
}
};

int main() {
    srand(time(nullptr));

    int numTellers;
    cout << "Enter the number of tellers: ";
    cin >> numTellers;

    Bank bank(numTellers);

    int numCustomers;
    cout << "Enter the number of customers: ";
    cin >> numCustomers;


    for (int i = 1; i <= numCustomers; i++) {
        int arrivalTime;
        cout << "Enter arrival time for customer " << i << ": ";
        cin >> arrivalTime;

        int serviceTime;
        cout << "Enter service time for customer " << i << ": ";
        cin >> serviceTime;

        Customer* customer = new Customer(i, arrivalTime, serviceTime);
        bank.addCustomer(customer);
    }


    bank.displayQueueStatus();


    bank.serveCustomers();




    bank.displayStatistics();

    return 0;
}
