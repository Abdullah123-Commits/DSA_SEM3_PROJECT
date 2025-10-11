#include <iostream>
#include <string>
using namespace std;

class Item {
    private:
        // attributes
        int id;
        string name;
        double unitPrice;
        int quantity;       // QUANTITY MAY BHI CONFUSION HA; I THINK KISI AUR CLASS MAY HONA CHAHIYE THA LIKE INVENTORY
        int soldCount;    // CONFUSED ABOUT THIS PROPERTY ; ISKO YAHAN HONA CHAHYE YA KISI AUR CLASS MAY ! 
    public:
        // Constructor
        Item(int id, string name, double unitPrice, int quantity, int soldCount) {
            // i am not sure about id ; whether to implement it this way or to make a func that assign unique id to each item
            this->id = id;
            this->name = name;
            this->unitPrice = unitPrice;
            this->quantity = quantity;
            this->soldCount = soldCount;
        }
        // getters
        int getId() { return id; }
        string getName() { return name; }
        double getUnitPrice() { return unitPrice; }
        int getQuantity() { return quantity; }
        int getSoldCount() { return soldCount; }
        // setters
        void setId(int id) { this->id = id; }
        void setName(string name) { this->name = name; }
        void setUnitPrice(double unitPrice) { this->unitPrice = unitPrice; }
        void setQuantity(int quantity) { this->quantity = quantity; } 
        void setSoldCount(int soldCount) {this->soldCount = soldCount; }
};

class Inventory {
    private:
        // data members
        Item* list;
        
};

int main() {
    return 0;
}