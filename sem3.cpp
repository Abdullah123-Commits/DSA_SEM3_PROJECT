#include <iostream>
#include <string>
using namespace std;

class Date {
    private:
        // data members
        int day;
        int month;
        int year;
    public:
        // default constructor
        Date() {}
        // methods
        Date(int day, int month, int year) {
            this->day = day;
            this->month = month;
            this->year = year;
        }
        // getters
        int getDay() const { return day; }
        int getMonnth() const { return month; }
        int getYear() const { return year; }
        // setters
        void setDay(int day) { this->day = day; }
        void setMonth(int month) { this->month = month; }
        void setYear(int year) { this->year = year; }
        // display date
        void printDate() {
            cout << "Date: " << day << "-" << month << "-" << year << endl;
            return;
        }
};

class Item {
    private:
        // attributes
        int id;
        string name;
        double unitPrice;
        int quantity;       // QUANTITY MAY BHI CONFUSION HA; I THINK KISI AUR CLASS MAY HONA CHAHIYE THA LIKE INVENTORY
        int soldCount;    // CONFUSED ABOUT THIS PROPERTY ; ISKO YAHAN HONA CHAHYE YA KISI AUR CLASS MAY ! 
        Date mfgDate;
        Date expDate;
    public:
        // default constructor
        Item() { }
        // Constructor
        Item(int id, string name, double unitPrice, int quantity, int soldCount, int mfgDay, int mfgMon, int mfgYear,
            int expDay, int expMon, int expYear) : mfgDate(mfgDay, mfgMon, mfgYear), expDate(expDay,expMon,expYear) {
            // i am not sure about id ; whether to implement it this way or to make a func that assign unique id to each item
            this->id = id;
            this->name = name;
            this->unitPrice = unitPrice;
            this->quantity = quantity;
            this->soldCount = soldCount;
        }
        // getters
        int getId() const { return id; }
        string getName() const { return name; }
        double getUnitPrice() const { return unitPrice; }
        int getQuantity() const { return quantity; }
        int getSoldCount() const { return soldCount; }
        // setters
        void setId(int id) { this->id = id; }
        void setName(string name) { this->name = name; }
        void setUnitPrice(double unitPrice) { this->unitPrice = unitPrice; }
        void setQuantity(int quantity) { this->quantity = quantity; } 
        void setSoldCount(int soldCount) {this->soldCount = soldCount; }
        // print mfg date
        void printMfgDate() { mfgDate.printDate(); }
        void printExpDate() { expDate.printDate(); }
};

class Inventory {
    private:
        // data members
        int count=0;    // static variable to keep the item count in the inventory; removed static ; it will be illogical in case of multiple inventory
        int currentSpace=0;
        Item* list;
    public:
        // methods     * THIS CLASS WOULD NEED RULE OF THREE 
        // getters
        int getCount() const { return count; }
        int getCurrentSpace() const { return currentSpace; }
        Inventory(int space) {
            this->list = new Item[space]; // allocate space as needed; 
        }
        // copy constructor
        Inventory (const Inventory& other) {
            this->count = other.getCount();
            this->list = NULL;
            this->currentSpace = other.getCurrentSpace();
            this->list = new Item[this->currentSpace];
            // after allocation copy the data
            for (int i=0; i<other.getCurrentSpace; i++) {
                this->list[i] = other.list[i];
            }
        } 
        
};


int main() {
    return 0;
}