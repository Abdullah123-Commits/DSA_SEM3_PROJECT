// ADD TEVENUE DATA ATTRIBUTE IN ITEM CLASS IMP
// MAKE FUNCTION FOR SELLITEM()
// UNDO WITH STACK (BAAD MAY DEKHLENA)
// IMPLEMENTING MERGE/ QUICK SORT()
// BST FOR FAST SEARCHING
// TOP SELLER / LEAST SELLER TOTAL REVENUE
//
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
        int getMonth() const { return month; }
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
        double revenue;
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
            this->revenue = 0;
            this->name = name;
            this->unitPrice = unitPrice;
            this->quantity = quantity;
            this->soldCount = soldCount;
        }
        // getters
        int getId() const { return id; }
        double getRevenue() const { return revenue; }
        string getName() const { return name; }
        double getUnitPrice() const { return unitPrice; }
        int getQuantity() const { return quantity; }
        int getSoldCount() const { return soldCount; }
        // setters
        void setId(int id) { this->id = id; }
        void setRevenue(double value) { revenue += value; }
        void setName(string name) { this->name = name; }
        void setUnitPrice(double unitPrice) { this->unitPrice = unitPrice; }
        void setQuantity(int quantity) { this->quantity = quantity; }
        void setSoldCount(int soldCount) {this->soldCount = soldCount; }
        // print mfg date
        void printMfgDate() { mfgDate.printDate(); }
        void printExpDate() { expDate.printDate(); }
        // print func for item info
        void printItemInfo() {
            cout << "ID: " << getId() << " | Name: " << getName() << "| unit Price: " << getUnitPrice() << endl;
            printMfgDate();printExpDate();
        }
};

class ItemNode {
    public:
        Item item;
        ItemNode* next;
        ItemNode* prev;
    // constructor
    ItemNode(const Item& obj) {
        this->item = obj;
        this->next = NULL;
        this->prev = NULL;
    }
};


class Inventory {
    private:
        // data members
        int count=0;    // static variable to keep the item count in the inventory; removed static ; it will be illogical in case of multiple inventory
        // int currentSpace=0; i guess yetab banaya tha when we did not incorporated linkedlists
        ItemNode* head; // doubly linked list
        ItemNode* tail;
    public:
        // getters
        int getCount() const { return count; }
        // int getCurrentSpace() const { return currentSpace; }
        // simple constructor
        Inventory() {
            this->head = NULL; // the list will be empty initially
            this->tail = NULL;
        }
        // FUNCTION to add an item to the list
        void addItem(const Item& itemToBeAdded) {
            ItemNode* newEntry = new ItemNode(itemToBeAdded);
            // Case # 1: if the list is empty then insert @ head
            if (head == NULL) {
                head = newEntry;
                tail = newEntry;    // if no of elements in list =1 then head = tail
                count+=1;
                return;
            }
            // Case # 2: if list is not empty then insert @ tail
            tail->next = newEntry;
            tail = newEntry;
            count +=1;
            return;
        }
        // FUNCTION to remove item by id
        void removeItemById(int id) {
            if (head->item.getId() == id) {
                removeAtHead();
                count-=1;
                return;
            }
            ItemNode* current = head;
            while(current->next!=NULL) {
                if (current->item.getId() == id) {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    delete current;
                    count-=1;
                    return;
                }
                current = current->next;
            }
            // tail ki deletion
            if (current->item.getId() == id) {
                removeAtTail();
                count-=1;
                return;
            }
            cout << "Item of specified ID is not found in the inventory!" << endl;
            return;
        }
        // remove at head
        void removeAtHead() {
            //  Case 1 if wala body agar list ka asize = 1 ho OR head = tail
            ItemNode *temp=head;
            if (temp!=NULL && temp == tail) {
                head = NULL;
                tail = NULL;
                delete temp;
                return;
            }
            //case 2 simple head deletion
            else if(temp!=NULL) {
                head = temp->next;
                head->prev = NULL;
                delete temp;
                return;
            } else {
                cout << "list is empty" << endl;
                return;
            }
        }
        // remove at tail
        void removeAtTail() {
            ItemNode *temp=tail;
            if (head == NULL) {
                cout << "list empty";
                return;
            }
            if (head==tail) {
                head = NULL;
                tail = NULL;
                delete temp;
                return;
            }
            // simple tail deletion
            tail = temp->prev;
            tail->next = NULL;
            delete temp;
            return;
        }
        // display FUNCTION TO PRINT THE INVENTORY
        void printInventory() {
            ItemNode* temp = head;
            while(temp!=NULL) {
                temp->item.printItemInfo();
                temp = temp->next;
            }
            return;
        }
        // FUNCTION TO UPDATE THE INFO OF ANY EXISTING ITEM
        void updateItemPrice(int id) {
            double newPrice;
            if (isFound(id)) {
                cout << "Enter new price for the item having ID:" << id;
                cin >> newPrice;
                ItemNode* temp = head;
                while (temp!=NULL) {
                    if (temp->item.getId() == id) {
                        temp->item.setUnitPrice(newPrice);
                        return;
                    }
                    temp = temp->next;
                }
            } else {
                cout << "There is no item with ID" << id << "in the inventory" << endl;
                return;
            }
        }
        // linear search based ISFOUND() function
        bool isFound(int id) {
            ItemNode* temp = head;
            while (temp != NULL) {
            if (temp->item.getId() == id) {
            return true;
            }
            temp = temp->next;
            }
            return false;
        }
        // FUNCTION TO SIMULATE SELLITEM()
        void sellItem(int id, int quantityToSell) {
            ItemNode *temp = head;
            while(temp!=NULL) {
                if (temp->item.getId() == id) {
                    if (isAvailable(id, quantityToSell)) {
                        double unitPrice = temp->item.getUnitPrice();
                        int currentQuantity = temp->item.getQuantity();
                        double currentRevenue = temp->item.getRevenue();
                        temp->item.setQuantity(currentQuantity-quantityToSell);
                        temp->item.setRevenue(currentRevenue+(quantityToSell*unitPrice));
                        cout << "Item sold successfully" << endl;
                        return;
                    } else {
                        cout << "Not available in the desired amount\n";
                        return;
                    }
                }
                    temp = temp->next;
                }
            cout<<"Invalid ID entered !\n";
            return; 
        }
        // isAvailable()
        bool isAvailable(int id, int amountRequired) {
            ItemNode* temp = head;
            if (isFound(id)) {
                while(temp!=NULL) {
                    if (temp->item.getId() == id && temp->item.getQuantity() > amountRequired) {
                        return true;
                    }
                    temp = temp->next;
                }
            }
            return false;
        }
        // DESTRUCTOR
        ~Inventory() {
                    ItemNode* temp = head;
                    while (temp) {
                        ItemNode* toDelete = temp;
                        temp = temp->next;
                        delete toDelete;
                    }
                }

};


int main() {
    return 0;
}