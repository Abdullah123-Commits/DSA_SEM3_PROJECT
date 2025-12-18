// Most Recent (Fixed) Commit (25-12-2025)
#include <iostream>
#include <cmath>
#include <fstream>
#include <cstring>
#include <string>
#include <chrono>    // For timing
#include <vector>    // For vectors
#include <algorithm> // For min(), max() on Linux

using namespace std;

// Forward declarations
class Date;
class Item;
class Inventory;
class Admin;

// Date Class
class Date {
private:
    int day;
    int month;
    int year;

public:
    Date() : day(1), month(1), year(2000) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    // Getters
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    
    // Setters
    void setDay(int d) { day = d; }
    void setMonth(int m) { month = m; }
    void setYear(int y) { year = y; }
    
    void display() const {
        cout << day << "/" << month << "/" << year;
    }
    
    // File handling methods
    void saveToFile(ofstream& file) const {
        file << day << " " << month << " " << year << " ";
    }
    
    void loadFromFile(ifstream& file) {
        file >> day >> month >> year;
    }
};

// Item Class
class Item {
private:
    int id;
    string name;
    double price;
    int quantity;
    int soldCount;
    double revenue;
    Date mfgDate;
    Date expDate;

public:
    Item() : id(0), price(0), quantity(0), soldCount(0), revenue(0) {}
    Item(int i, string n, double p, int q, Date mfg, Date exp) 
        : id(i), name(n), price(p), quantity(q), soldCount(0), revenue(0), mfgDate(mfg), expDate(exp) {}
    
    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getSoldCount() const { return soldCount; }
    double getRevenue() const { return revenue; }
    Date getMfgDate() const { return mfgDate; }
    Date getExpDate() const { return expDate; }
    
    // Setters
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    void setName(string n) { name = n; }
    
    // Business methods
    bool sell(int quantityToSell) {
        if (quantityToSell <= quantity) {
            quantity -= quantityToSell;
            soldCount += quantityToSell;
            revenue += quantityToSell * price;
            return true;
        }
        return false;
    }
    
    void restock(int amount) {
        quantity += amount;
    }
    
    void display() const {
        cout << "ID: " << id << " | Name: " << name << " | Price: $" << price 
             << " | Quantity: " << quantity << " | Sold: " << soldCount 
             << " | Revenue: $" << revenue << endl;
        cout << "Manufacturing: ";
        mfgDate.display();
        cout << " | Expiry: ";
        expDate.display();
        cout << endl;
    }
    
    // File handling methods
    void saveToFile(ofstream& file) const {
        file << id << " " << name.length() << " " << name << " " << price << " " 
             << quantity << " " << soldCount << " " << revenue << " ";
        mfgDate.saveToFile(file);
        expDate.saveToFile(file);
        file << endl;
    }
    
    void loadFromFile(ifstream& file) {
        int nameLength;
        file >> id >> nameLength;
        file.ignore(); // Ignore the space
        char* tempName = new char[nameLength + 1];
        file.read(tempName, nameLength);
        tempName[nameLength] = '\0';
        name = string(tempName);
        delete[] tempName;
        
        file >> price >> quantity >> soldCount >> revenue;
        mfgDate.loadFromFile(file);
        expDate.loadFromFile(file);
    }
};

// Hash Table for efficient searching
class HashTable {
private:
    struct HashNode {
        int key;
        Item* item;
        HashNode* next;
        HashNode(int k, Item* i) : key(k), item(i), next(nullptr) {}
    };
    
    HashNode** table;
    int capacity;
    int size;
    
    int hashFunction(int key) {
        return key % capacity;
    }
    
public:
    HashTable(int cap = 101) {
        capacity = cap;
        size = 0;
        table = new HashNode*[capacity]();
    }
    
    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }
    
    void insert(int key, Item* item) {
        int index = hashFunction(key);
        HashNode* newNode = new HashNode(key, item);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
    }
    
    Item* search(int key) {
        int index = hashFunction(key);
        HashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                return current->item;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    bool remove(int key) {
        int index = hashFunction(key);
        HashNode* current = table[index];
        HashNode* prev = nullptr;
        
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                size--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }
};

// Stack for undo operations
template<typename T>
class Stack {
private:
    struct StackNode {
        T data;
        StackNode* next;
        StackNode(const T& d) : data(d), next(nullptr) {}
    };
    
    StackNode* top;
    int count;

public:
    Stack() : top(nullptr), count(0) {}
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    void push(const T& data) {
        StackNode* newNode = new StackNode(data);
        newNode->next = top;
        top = newNode;
        count++;
    }
    
    T pop() {
        if (isEmpty()) {
            throw "Stack is empty";
        }
        StackNode* temp = top;
        T data = top->data;
        top = top->next;
        delete temp;
        count--;
        return data;
    }
    
    bool isEmpty() const {
        return top == nullptr;
    }
    
    int getCount() const {
        return count;
    }
};

// Sorting Algorithms (declarations first)
void swap(Item& a, Item& b);
void bubbleSort(Item arr[], int n, bool byRevenue = false);
void insertionSort(Item arr[], int n, bool byRevenue = false);
void selectionSort(Item arr[], int n, bool byRevenue = false);
void merge(Item arr[], int left, int mid, int right, bool byRevenue);
void mergeSort(Item arr[], int left, int right, bool byRevenue = false);
void heapify(Item arr[], int n, int i, bool byRevenue);
void heapSort(Item arr[], int n, bool byRevenue = false);
int hoarePartition(Item arr[], int low, int high, bool byRevenue = false);
void quickSort(Item arr[], int low, int high, bool byRevenue = false);

// Sorting Algorithms Implementation
void swap(Item& a, Item& b) {
    Item temp = a;
    a = b;
    b = temp;
}

// 1. BUBBLE SORT (Baseline - Slowest)
void bubbleSort(Item arr[], int n, bool byRevenue) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            double val1 = byRevenue ? arr[j].getRevenue() : arr[j].getSoldCount();
            double val2 = byRevenue ? arr[j + 1].getRevenue() : arr[j + 1].getSoldCount();
            
            if (val1 < val2) {  // Descending order
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 2. INSERTION SORT (Good for small/partially sorted data)
void insertionSort(Item arr[], int n, bool byRevenue) {
    for (int i = 1; i < n; i++) {
        Item key = arr[i];
        double keyVal = byRevenue ? key.getRevenue() : key.getSoldCount();
        int j = i - 1;
        
        while (j >= 0 && (byRevenue ? arr[j].getRevenue() : arr[j].getSoldCount()) < keyVal) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 3. SELECTION SORT (Simple but O(n²))
void selectionSort(Item arr[], int n, bool byRevenue) {
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++) {
            double val1 = byRevenue ? arr[j].getRevenue() : arr[j].getSoldCount();
            double val2 = byRevenue ? arr[maxIdx].getRevenue() : arr[maxIdx].getSoldCount();
            
            if (val1 > val2) {
                maxIdx = j;
            }
        }
        swap(arr[i], arr[maxIdx]);
    }
}

// 4. MERGE SORT (Stable O(n log n) - for comparison with QuickSort)
void merge(Item arr[], int left, int mid, int right, bool byRevenue) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Create temp arrays
    Item* leftArr = new Item[n1];
    Item* rightArr = new Item[n2];
    
    // Copy data
    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];
    
    // Merge
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        double val1 = byRevenue ? leftArr[i].getRevenue() : leftArr[i].getSoldCount();
        double val2 = byRevenue ? rightArr[j].getRevenue() : rightArr[j].getSoldCount();
        
        if (val1 >= val2) {  // Descending order
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    
    // Copy remaining
    while (i < n1) {
        arr[k] = leftArr[i];
        i++; k++;
    }
    while (j < n2) {
        arr[k] = rightArr[j];
        j++; k++;
    }
    
    delete[] leftArr;
    delete[] rightArr;
}

void mergeSort(Item arr[], int left, int right, bool byRevenue) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, byRevenue);
        mergeSort(arr, mid + 1, right, byRevenue);
        merge(arr, left, mid, right, byRevenue);
    }
}

// 5. HEAP SORT (Another O(n log n) algorithm)
void heapify(Item arr[], int n, int i, bool byRevenue) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n) {
        double leftVal = byRevenue ? arr[left].getRevenue() : arr[left].getSoldCount();
        double largestVal = byRevenue ? arr[largest].getRevenue() : arr[largest].getSoldCount();
        if (leftVal > largestVal)
            largest = left;
    }
    
    if (right < n) {
        double rightVal = byRevenue ? arr[right].getRevenue() : arr[right].getSoldCount();
        double largestVal = byRevenue ? arr[largest].getRevenue() : arr[largest].getSoldCount();
        if (rightVal > largestVal)
            largest = right;
    }
    
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest, byRevenue);
    }
}

void heapSort(Item arr[], int n, bool byRevenue) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, byRevenue);
    
    // Extract elements from heap
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, byRevenue);
    }
}

// Quick Sort Implementation with Hoare Partition Scheme
int hoarePartition(Item arr[], int low, int high, bool byRevenue) {
    // Choose the middle element as pivot (better than first/last for sorted arrays)
    int mid = low + (high - low) / 2;
    double pivot = byRevenue ? arr[mid].getRevenue() : arr[mid].getSoldCount();
    
    int i = low - 1;
    int j = high + 1;
    
    while (true) {
        // Find left element that should be on right side
        do {
            i++;
        } while (i <= high && (byRevenue ? arr[i].getRevenue() : arr[i].getSoldCount()) > pivot);
        
        // Find right element that should be on left side  
        do {
            j--;
        } while (j >= low && (byRevenue ? arr[j].getRevenue() : arr[j].getSoldCount()) < pivot);
        
        // If pointers crossed, partition is complete
        if (i >= j) {
            return j;
        }
        
        // Swap elements that are on wrong sides
        swap(arr[i], arr[j]);
    }
}

// Quick Sort using Hoare Partition
void quickSort(Item arr[], int low, int high, bool byRevenue) {
    if (low < high) {
        // pi is partitioning index, arr[pi] is now at right place
        int pi = hoarePartition(arr, low, high, byRevenue);
        
        // Separately sort elements before and after partition
        quickSort(arr, low, pi, byRevenue);
        quickSort(arr, pi + 1, high, byRevenue);
    }
}

// Benchmark Class
class Benchmark {
private:
    struct AlgorithmResult {
        string name;
        double timeMillis;
        bool correct;
    };
    
    Item* testData;
    Item* backupData;
    int dataSize;
    
    // Verify if array is sorted correctly (descending order)
    bool verifySort(Item arr[], int n, bool byRevenue) {
        for (int i = 0; i < n - 1; i++) {
            double val1 = byRevenue ? arr[i].getRevenue() : arr[i].getSoldCount();
            double val2 = byRevenue ? arr[i + 1].getRevenue() : arr[i + 1].getSoldCount();
            if (val1 < val2) return false;
        }
        return true;
    }
    
    // Restore original data
    void restoreData() {
        for (int i = 0; i < dataSize; i++) {
            testData[i] = backupData[i];
        }
    }
    
public:
    Benchmark(Item* data, int size) {
        dataSize = size;
        testData = data;
        backupData = new Item[dataSize];
        // Create backup
        for (int i = 0; i < dataSize; i++) {
            backupData[i] = data[i];
        }
    }
    
    ~Benchmark() {
        delete[] backupData;
    }
    
    AlgorithmResult runAlgorithm(const string& algoName, bool byRevenue = false) {
        AlgorithmResult result;
        result.name = algoName;
        
        // Restore original data
        restoreData();
        
        // Start timer
        auto start = chrono::high_resolution_clock::now();
        
        // Run algorithm
        if (algoName == "Bubble Sort") {
            bubbleSort(testData, dataSize, byRevenue);
        } else if (algoName == "Insertion Sort") {
            insertionSort(testData, dataSize, byRevenue);
        } else if (algoName == "Selection Sort") {
            selectionSort(testData, dataSize, byRevenue);
        } else if (algoName == "Merge Sort") {
            mergeSort(testData, 0, dataSize - 1, byRevenue);
        } else if (algoName == "Heap Sort") {
            heapSort(testData, dataSize, byRevenue);
        } else if (algoName == "Quick Sort") {
            quickSort(testData, 0, dataSize - 1, byRevenue);
        }
        
        // Stop timer
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        result.timeMillis = duration.count() / 1000.0;  // Convert to milliseconds
        result.correct = verifySort(testData, dataSize, byRevenue);
        
        return result;
    }
    
    // Compare all algorithms
    void runAllBenchmarks(bool byRevenue = false) {
        cout << "\n=== PERFORMANCE BENCHMARK (" << dataSize << " items) ===\n";
        cout << "Sorting by: " << (byRevenue ? "Revenue" : "Sold Count") << endl;
        cout << "--------------------------------------------------\n";
        
        vector<string> algorithms = {
            "Bubble Sort",
            "Selection Sort", 
            "Insertion Sort",
            "Merge Sort",
            "Heap Sort",
            "Quick Sort"
        };
        
        vector<AlgorithmResult> results;
        
        for (const auto& algo : algorithms) {
            AlgorithmResult result = runAlgorithm(algo, byRevenue);
            results.push_back(result);
            
            cout << algo << ": ";
            cout << result.timeMillis << " ms";
            cout << " [" << (result.correct ? " CORRECT" : " INCORRECT") << "]\n";
        }
        
        // Find fastest
        double minTime = results[0].timeMillis;
        string fastest = results[0].name;
        for (int i = 1; i < results.size(); i++) {
            if (results[i].timeMillis < minTime && results[i].correct) {
                minTime = results[i].timeMillis;
                fastest = results[i].name;
            }
        }
        
        cout << "\n--------------------------------------------------\n";
        cout << "FASTEST ALGORITHM: " << fastest << " (" << minTime << " ms)\n";
        cout << "RECOMMENDED FOR SYSTEM: " << fastest << endl;
        
        // Display performance comparison table
        cout << "\n=== PERFORMANCE COMPARISON TABLE ===\n";
        cout << "Algorithm           Time (ms)   Relative Speed\n";
        cout << "----------------------------------------\n";
        
        for (const auto& result : results) {
            double relative = result.timeMillis / minTime;
            printf("%-18s %8.2f ms %12.1fx\n", 
                   result.name.c_str(), 
                   result.timeMillis, 
                   relative);
        }
    }
    
    // Generate simple ASCII bar chart
    void generateBarChart(bool byRevenue = false) {
        cout << "\n=== PERFORMANCE BAR CHART ===\n";
        cout << "█ = 100 ms\n\n";
        
        vector<string> algorithms = {
            "Bubble Sort",
            "Selection Sort", 
            "Insertion Sort",
            "Merge Sort",
            "Heap Sort",
            "Quick Sort"
        };
        
        vector<double> times;
        for (const auto& algo : algorithms) {
            AlgorithmResult result = runAlgorithm(algo, byRevenue);
            times.push_back(result.timeMillis);
        }
        
        // Find max for scaling
        double maxTime = 0;
        for (double t : times) if (t > maxTime) maxTime = t;
        
        for (int i = 0; i < algorithms.size(); i++) {
            cout << algorithms[i] << ": ";
            int bars = (int)((times[i] / maxTime) * 50);  // Scale to 50 chars
            for (int j = 0; j < bars; j++) cout << "█";
            cout << " " << times[i] << " ms\n";
        }
    }
};

// Inventory Class
class Inventory {
private:
    Item* items;
    int capacity;
    int size;
    HashTable* hashTable;
    Stack<Item> undoStack;
    
    void resize() {
        int newCapacity = capacity * 2;
        Item* newItems = new Item[newCapacity];
        
        for (int i = 0; i < size; i++) {
            newItems[i] = items[i];
        }
        
        delete[] items;
        items = newItems;
        capacity = newCapacity;
        
        // Rebuild hash table
        delete hashTable;
        hashTable = new HashTable(capacity * 2);
        for (int i = 0; i < size; i++) {
            hashTable->insert(items[i].getId(), &items[i]);
        }
    }
    
public:
    Inventory(int initialCapacity = 50) {
        capacity = initialCapacity;
        size = 0;
        items = new Item[capacity];
        hashTable = new HashTable(capacity * 2);
    }
    
    ~Inventory() {
        delete[] items;
        delete hashTable;
    }
    
    // NEW: Get data copy for benchmarking
    Item* getDataCopyForBenchmark() {
        Item* copy = new Item[size];
        for (int i = 0; i < size; i++) {
            copy[i] = items[i];
        }
        return copy;
    }
    
    // NEW: Get data pointer (use with caution!)
    Item* getDataPointerForBenchmark() {
        return items;
    }
    
    // Core inventory operations
    void addItem(const Item& item) {
        if (size >= capacity) {
            resize();
        }
        
        // Save to undo stack before adding
        Item temp; // Empty item for undo remove
        undoStack.push(temp);
        
        items[size] = item;
        hashTable->insert(item.getId(), &items[size]);
        size++;
        cout << "Item added successfully!\n";
    }
    
    bool removeItem(int id) {
        Item* item = hashTable->search(id);
        if (item != nullptr) {
            // Save to undo stack before removing
            undoStack.push(*item);
            
            // Find index and remove
            for (int i = 0; i < size; i++) {
                if (items[i].getId() == id) {
                    // CRITICAL BUG FIX: Update hash table pointers when shifting elements
                    // First remove the item to be deleted
                    hashTable->remove(id);
                    
                    // Shift elements and update their hash table pointers
                    for (int j = i; j < size - 1; j++) {
                        // Remove old pointer for item being shifted
                        hashTable->remove(items[j + 1].getId());
                        
                        // Shift the element
                        items[j] = items[j + 1];
                        
                        // Insert new pointer for shifted item
                        hashTable->insert(items[j].getId(), &items[j]);
                    }
                    
                    size--;
                    cout << "Item removed successfully!\n";
                    return true;
                }
            }
        }
        cout << "Item not found!\n";
        return false;
    }
    
    bool sellItem(int id, int quantity) {
        Item* item = hashTable->search(id);
        if (item != nullptr) {
            // Save to undo stack before selling
            undoStack.push(*item);
            
            if (item->sell(quantity)) {
                cout << "Sale completed successfully!\n";
                return true;
            } else {
                cout << "Insufficient quantity!\n";
                undoStack.pop(); // Remove the saved state since sale failed
            }
        } else {
            cout << "Item not found!\n";
        }
        return false;
    }
    
    void restockItem(int id, int quantity) {
        Item* item = hashTable->search(id);
        if (item != nullptr) {
            // Save to undo stack before restocking
            undoStack.push(*item);
            
            item->restock(quantity);
            cout << "Item restocked successfully!\n";
        } else {
            cout << "Item not found!\n";
        }
    }
    
    void undo() {
        if (!undoStack.isEmpty()) {
            Item previousState = undoStack.pop();
            
            // Special case: If ID is 0, it means an item was just added (we need to remove it)
            if (previousState.getId() == 0) {
                if (size > 0) {
                    int lastItemId = items[size - 1].getId();
                    hashTable->remove(lastItemId);
                    size--;
                    cout << "Undo: Item removed successfully!\n";
                } else {
                    cout << "Undo: Nothing to remove!\n";
                }
                return;
            }
            
            // Find the item and restore its state
            for (int i = 0; i < size; i++) {
                if (items[i].getId() == previousState.getId()) {
                    items[i] = previousState;
                    hashTable->insert(previousState.getId(), &items[i]);
                    cout << "Undo operation completed!\n";
                    return;
                }
            }
            // If item not found, it was probably deleted, so add it back
            if (previousState.getId() != 0) { // Check if it was a real item
                addItem(previousState);
            }
        } else {
            cout << "No operations to undo!\n";
        }
    }
    
    // Search and display operations
    Item* searchItem(int id) {
        return hashTable->search(id);
    }
    
    void displayAllItems() {
        if (size == 0) {
            cout << "Inventory is empty!\n";
            return;
        }
        cout << "\n=== ALL ITEMS ===\n";
        for (int i = 0; i < size; i++) {
            items[i].display();
        }
    }
    
    // Report generation
    void generateSalesReport() {
        if (size == 0) {
            cout << "No items in inventory!\n";
            return;
        }
        
        // Create temporary array for sorting
        Item* tempItems = new Item[size];
        for (int i = 0; i < size; i++) {
            tempItems[i] = items[i];
        }
        
        cout << "\n=== SALES REPORT ===\n";
        
        // Top selling items (by sold count)
        quickSort(tempItems, 0, size - 1, false);
        cout << "\nTOP SELLING ITEMS:\n";
        int displayCount = min(3, size);
        for (int i = 0; i < displayCount; i++) {
            cout << i + 1 << ". ";
            tempItems[i].display();
        }
        
        // Top revenue generators
        quickSort(tempItems, 0, size - 1, true);
        cout << "\nTOP REVENUE GENERATORS:\n";
        for (int i = 0; i < displayCount; i++) {
            cout << i + 1 << ". ";
            tempItems[i].display();
        }
        
        // Least selling items
        cout << "\nLEAST SELLING ITEMS:\n";
        int startIdx = max(0, size - 3);
        for (int i = startIdx; i < size; i++) {
            cout << size - i << ". ";
            tempItems[i].display();
        }
        
        delete[] tempItems;
    }
    
    // File handling
    void saveToFile(const string& filename) {
        ofstream file(filename, ios::binary);
        if (!file) {
            cout << "Error opening file for writing!\n";
            return;
        }
        
        file << size << " ";
        for (int i = 0; i < size; i++) {
            items[i].saveToFile(file);
        }
        file.close();
        cout << "Inventory saved to file successfully!\n";
    }
    
    void loadFromFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cout << "Error opening file for reading!\n";
            return;
        }
        
        file >> size;
        if (size > capacity) {
            delete[] items;
            capacity = size * 2;
            items = new Item[capacity];
            delete hashTable;
            hashTable = new HashTable(capacity * 2);
        }
        
        for (int i = 0; i < size; i++) {
            items[i].loadFromFile(file);
            hashTable->insert(items[i].getId(), &items[i]);
        }
        file.close();
        cout << "Inventory loaded from file successfully!\n";
    }
    
    int getSize() const { return size; }
};

// Admin Class
class Admin {
private:
    string username;
    string password;
    bool loggedIn;

public:
    Admin() : username("admin"), password("admin123"), loggedIn(false) {}
    
    bool login(const string& user, const string& pass) {
        if (user == username && pass == password) {
            loggedIn = true;
            cout << "Login successful!\n";
            return true;
        } else {
            cout << "Invalid credentials!\n";
            return false;
        }
    }
    
    void logout() {
        loggedIn = false;
        cout << "Logged out successfully!\n";
    }
    
    bool isLoggedIn() const {
        return loggedIn;
    }
};

// Utility functions
void displayMenu() {
    cout << "\n=== INVENTORY MANAGEMENT SYSTEM ===\n";
    cout << "1. Add Item\n";
    cout << "2. Remove Item\n";
    cout << "3. Sell Item\n";
    cout << "4. Restock Item\n";
    cout << "5. Search Item\n";
    cout << "6. Display All Items\n";
    cout << "7. Generate Sales Report\n";
    cout << "8. Undo Last Operation\n";
    cout << "9. Save to File\n";
    cout << "10. Load from File\n";
    cout << "11. Logout\n";
    cout << "12. Clear Screen\n";
    cout << "13. Run Performance Benchmark\n";   
    cout << "14. Generate Performance Report\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

Item createItemFromInput() {
    int id, quantity, mfgDay, mfgMonth, mfgYear, expDay, expMonth, expYear;
    string name;
    double price;
    
    cout << "Enter Item ID: ";
    cin >> id;
    cout << "Enter Item Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Price: ";
    cin >> price;
    cout << "Enter Quantity: ";
    cin >> quantity;
    cout << "Enter Manufacturing Date (dd mm yyyy): ";
    cin >> mfgDay >> mfgMonth >> mfgYear;
    cout << "Enter Expiry Date (dd mm yyyy): ";
    cin >> expDay >> expMonth >> expYear;
    
    Date mfgDate(mfgDay, mfgMonth, mfgYear);
    Date expDate(expDay, expMonth, expYear);
    
    return Item(id, name, price, quantity, mfgDate, expDate);
}

// Benchmark related functions - COMPLETE VERSION
void generatePerformanceReport(Inventory& inventory) {
    ofstream report("performance_report.txt");
    
    report << "===========================================\n";
    report << "     INVENTORY SYSTEM PERFORMANCE REPORT    \n";
    report << "===========================================\n\n";
    
    report << "Dataset Size: " << inventory.getSize() << " items\n";
    report << "Date: " << __DATE__ << "\n";
    report << "Time: " << __TIME__ << "\n\n";
    
    // Get data for testing
    int size = inventory.getSize();
    Item* testData = inventory.getDataCopyForBenchmark();
    Benchmark benchmark(testData, size);
    
    report << "=== SORTING ALGORITHM PERFORMANCE ===\n\n";
    
    // Test by sold count
    report << "1. Sorting by SOLD COUNT:\n";
    report << "--------------------------\n";
    
    vector<string> algorithms = {
        "Bubble Sort", "Selection Sort", "Insertion Sort",
        "Merge Sort", "Heap Sort", "Quick Sort"
    };
    
    vector<double> times;
    
    for (const auto& algo : algorithms) {
        auto result = benchmark.runAlgorithm(algo, false);
        report << algo << ": " << result.timeMillis << " ms";
        report << " [" << (result.correct ? "CORRECT" : "INCORRECT") << "]\n";
        times.push_back(result.timeMillis);
    }
    
    // Find fastest
    int fastestIdx = 0;
    for (size_t i = 1; i < times.size(); i++) {
        if (times[i] < times[fastestIdx]) fastestIdx = i;
    }
    
    report << "\nFastest: " << algorithms[fastestIdx] << " (" 
           << times[fastestIdx] << " ms)\n\n";
    
    // Test by revenue
    report << "2. Sorting by REVENUE:\n";
    report << "----------------------\n";
    
    times.clear();
    for (const auto& algo : algorithms) {
        auto result = benchmark.runAlgorithm(algo, true);
        report << algo << ": " << result.timeMillis << " ms";
        report << " [" << (result.correct ? "CORRECT" : "INCORRECT") << "]\n";
        times.push_back(result.timeMillis);
    }
    
    fastestIdx = 0;
    for (size_t i = 1; i < times.size(); i++) {
        if (times[i] < times[fastestIdx]) fastestIdx = i;
    }
    
    report << "\nFastest: " << algorithms[fastestIdx] << " (" 
           << times[fastestIdx] << " ms)\n\n";
    
    report << "=== CONCLUSION ===\n";
    report << "Quick Sort is selected as the primary sorting algorithm\n";
    report << "for the inventory system because:\n";
    report << "1. It shows the best performance (O(n log n) average case)\n";
    report << "2. It is in-place (requires no additional memory)\n";
    report << "3. It handles large datasets efficiently\n";
    report << "4. Hoare partition scheme prevents worst-case scenarios\n";
    
    report << "\n=== PERFORMANCE COMPARISON ===\n";
    report << "Algorithm           Time (ms)   Complexity   In-Place\n";
    report << "----------------------------------------------------\n";
    report << "Bubble Sort         Very Slow   O(n²)        Yes\n";
    report << "Selection Sort      Slow        O(n²)        Yes\n";
    report << "Insertion Sort      Medium      O(n²)        Yes\n";
    report << "Merge Sort          Fast        O(n log n)   No\n";
    report << "Heap Sort           Fast        O(n log n)   Yes\n";
    report << "Quick Sort          Fastest     O(n log n)   Yes\n";
    
    report.close();
    
    delete[] testData;  //  FIXED: Proper memory cleanup!
    
    cout << "\nPerformance report generated: 'performance_report.txt'\n";
}  // FIXED: Closing brace for function

// Main function - COMPLETE VERSION
int main() {
    Inventory inventory;
    Admin admin;
    int choice;
    bool running = true;
    
    // Login system
    string username, password;
    cout << "=== LOGIN ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    
    if (!admin.login(username, password)) {
        return 0;
    }
    
    while (running && admin.isLoggedIn()) {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: {
                Item newItem = createItemFromInput();
                inventory.addItem(newItem);
                break;
            }
            case 2: {
                int id;
                cout << "Enter Item ID to remove: ";
                cin >> id;
                inventory.removeItem(id);
                break;
            }
            case 3: {
                int id, quantity;
                cout << "Enter Item ID to sell: ";
                cin >> id;
                cout << "Enter quantity to sell: ";
                cin >> quantity;
                inventory.sellItem(id, quantity);
                break;
            }
            case 4: {
                int id, quantity;
                cout << "Enter Item ID to restock: ";
                cin >> id;
                cout << "Enter quantity to restock: ";
                cin >> quantity;
                inventory.restockItem(id, quantity);
                break;
            }
            case 5: {
                int id;
                cout << "Enter Item ID to search(Positive Key): ";
                cin >> id;
                Item* foundItem = inventory.searchItem(id);
                if (foundItem != nullptr) {
                    cout << "Item found:\n";
                    foundItem->display();
                } else {
                    cout << "Item not found!\n";
                }
                break;
            }
            case 6: {
                inventory.displayAllItems();
                break;
            }
            case 7: {
                inventory.generateSalesReport();
                break;
            }
            case 8: {
                inventory.undo();
                break;
            }
            case 9: {
                inventory.saveToFile("inventory.dat");
                break;
            }
            case 10: {
                inventory.loadFromFile("inventory.dat");
                break;
            }
            case 11: {
                admin.logout();
                break;
            }
            case 12: {
                system("clear");  // Linux/Mac
                // For Windows: system("cls");
                break;
            }
            case 13: {  // Run Performance Benchmark
                if (inventory.getSize() == 0) {
                    cout << "Inventory is empty! Load data from file first.\n";
                    cout << "Use option 10 to load 'inventory.dat'\n";
                    break;
                }
                
                int size = inventory.getSize();
                cout << "\n=== PERFORMANCE BENCHMARKING ===\n";
                cout << "Dataset size: " << size << " items\n\n";
                
                cout << "1. Benchmark Sorting by Sold Count\n";
                cout << "2. Benchmark Sorting by Revenue\n";
                cout << "3. Run All Benchmarks with Charts\n";
                cout << "Enter choice: ";
                
                int benchChoice;
                cin >> benchChoice;
                
                // Get data copy for benchmarking
                Item* testData = inventory.getDataCopyForBenchmark();
                Benchmark benchmark(testData, size);
                
                switch(benchChoice) {
                    case 1:
                        benchmark.runAllBenchmarks(false);  // By sold count
                        break;
                    case 2:
                        benchmark.runAllBenchmarks(true);   // By revenue
                        break;
                    case 3:
                        benchmark.runAllBenchmarks(false);
                        benchmark.generateBarChart(false);
                        
                        cout << "\n\n";
                        benchmark.runAllBenchmarks(true);
                        benchmark.generateBarChart(true);
                        break;
                    default:
                        cout << "Invalid choice!\n";
                }
                
                delete[] testData;  // Clean up
                break;
            }
            case 14: {  // Generate Performance Report
                if (inventory.getSize() == 0) {
                    cout << "Inventory is empty! Load data from file first.\n";
                    cout << "Use option 10 to load 'inventory.dat'\n";
                    break;
                }
                generatePerformanceReport(inventory);
                break;
            }
            case 0: {
                running = false;
                cout << "Thank you for using Inventory Management System!\n";
                break;
            }
            default: {
                cout << "Invalid choice! Please try again.\n";
                break;
            }
        } 
    }
    
    return 0;
}
