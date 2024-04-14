#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

struct Product {
    string name;
    double price;
    int quantity;

    string serialize() const {
        return name + "," + to_string(price) + "," + to_string(quantity);
    }

    static Product deserialize(const string& data) {
        Product product;
        size_t pos1 = data.find(',');
        size_t pos2 = data.find(',', pos1 + 1);
        product.name = data.substr(0, pos1);
        product.price = stod(data.substr(pos1 + 1, pos2 - pos1 - 1));
        product.quantity = stoi(data.substr(pos2 + 1));
        return product;
    }
};

class Order {
private:
    int orderNumber;
    double totalAmount;
    bool isCompleted;
    list<Product> products;

public:
    Order(int num) : orderNumber(num), totalAmount(0.0), isCompleted(false) {}

    int getOrderNumber() const {
        return orderNumber;
    }

    double getTotalAmount() const {
        return totalAmount;
    }

    void markAsCompleted() {
        isCompleted = true;
    }

    bool getCompletionStatus() const {
        return isCompleted;
    }

    void addProduct() {
        Product product;
        cout << "Enter product name: ";
        cin >> product.name;
        cout << "Enter product price: ";
        cin >> product.price;
        cout << "Enter product quantity: ";
        cin >> product.quantity;
        products.push_back(product);
        updateTotalAmount();
    }

    void viewOrderDetails() const {
        cout << "Order Number: " << orderNumber << endl;
        cout << "Total Amount: $" << totalAmount << endl;
        cout << "Completed: " << (isCompleted ? "Yes" : "No") << endl;
        int index = 0;
        for (const auto& product : products) {
            cout << ++index << ". " << product.name << " - $" << product.price << " - Quantity: " << product.quantity << endl;
        }
    }

    void updateTotalAmount() {
        totalAmount = 0;
        for (const auto& product : products) {
            totalAmount += product.price * product.quantity;
        }
    }

    string serialize() const {
        string data = to_string(orderNumber) + "," + to_string(isCompleted) + "," + to_string(totalAmount);
        for (const auto& product : products) {
            data += "\n" + product.serialize();
        }
        return data;
    }

    static Order deserialize(const string& data) {
        stringstream ss(data);
        string line;
        getline(ss, line);
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        int orderNum = stoi(line.substr(0, pos1));
        bool isComp = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1)) > 0;
        double totAmount = stod(line.substr(pos2 + 1));

        Order order(orderNum);
        order.isCompleted = isComp;
        order.totalAmount = totAmount;

        while (getline(ss, line)) {
            if (!line.empty()) {
                order.products.push_back(Product::deserialize(line));
            }
        }
        return order;
    }

    static void findMostExpensiveOrder(const list<Order>& orders) {
        if (orders.empty()) {
            cout << "No orders available." << endl;
            return;
        }

        double maxAmount = 0.0;
        const Order* mostExpensiveOrder = nullptr;

        for (const auto& order : orders) {
            if (order.getTotalAmount() > maxAmount) {
                maxAmount = order.getTotalAmount();
                mostExpensiveOrder = &order;
            }
        }

        if (mostExpensiveOrder) {
            cout << "Most Expensive Order Details:" << endl;
            mostExpensiveOrder->viewOrderDetails();
        }
        else {
            cout << "Failed to find the most expensive order." << endl;
        }
    }
};

void saveOrdersToFile(const list<Order>& orders, const string& filename) {
    ofstream file(filename);
    for (const auto& order : orders) {
        file << order.serialize() << "\n\n";
    }
    file.close();
}

void loadOrdersFromFile(list<Order>& orders, const string& filename) {
    ifstream file(filename);
    string segment, orderData;
    while (getline(file, segment)) {
        if (!segment.empty()) {
            orderData += segment + "\n";
        }
        else {
            if (!orderData.empty()) {
                orders.push_back(Order::deserialize(orderData));
                orderData.clear();
            }
        }
    }
    if (!orderData.empty()) {  // Deserialize last order if file does not end with a newline
        orders.push_back(Order::deserialize(orderData));
    }
    file.close();
}

int main() {
    list<Order> orders;
    int orderCount = 0;
    string filename = "orders.txt";
    int choice;

    // Load existing orders from file
    loadOrdersFromFile(orders, filename);
    if (!orders.empty()) {
        orderCount = orders.back().getOrderNumber();
    }

    do {
        cout << "\nOnline Order System\n";
        cout << "1. Add Order\n";
        cout << "2. Add Product to Order\n";
        cout << "3. Mark Order as Completed\n";
        cout << "4. View Order Details\n";
        cout << "5. Find Most Expensive Order\n";
        cout << "6. Save Orders to File\n";
        cout << "7. Load Orders from File\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            orders.emplace_back(++orderCount);
            cout << "Order created. Order Number: " << orderCount << endl;
            break;
        case 2:
            if (!orders.empty()) {
                cout << "Enter Order Number: ";
                int num;
                cin >> num;
                auto it = find_if(orders.begin(), orders.end(), [num](const Order& o) { return o.getOrderNumber() == num; });
                if (it != orders.end()) {
                    it->addProduct();
                }
                else {
                    cout << "Order not found." << endl;
                }
            }
            else {
                cout << "No orders available. Please add an order first." << endl;
            }
            break;
        case 3:
            if (!orders.empty()) {
                cout << "Enter Order Number to mark as completed: ";
                int num;
                cin >> num;
                auto it = find_if(orders.begin(), orders.end(), [num](const Order& o) { return o.getOrderNumber() == num; });
                if (it != orders.end()) {
                    it->markAsCompleted();
                    cout << "Order marked as completed." << endl;
                }
                else {
                    cout << "Order not found." << endl;
                }
            }
            else {
                cout << "No orders available. Please add an order first." << endl;
            }
            break;
        case 4:
            if (!orders.empty()) {
                cout << "Enter Order Number to view details: ";
                int num;
                cin >> num;
                auto it = find_if(orders.begin(), orders.end(), [num](const Order& o) { return o.getOrderNumber() == num; });
                if (it != orders.end()) {
                    it->viewOrderDetails();
                }
                else {
                    cout << "Order not found." << endl;
                }
            }
            else {
                cout << "No orders available to view." << endl;
            }
            break;
        case 5:
            Order::findMostExpensiveOrder(orders);
            break;
        case 6:
            saveOrdersToFile(orders, filename);
            cout << "Orders saved successfully." << endl;
            break;
        case 7:
            loadOrdersFromFile(orders, filename);
            cout << "Orders loaded successfully." << endl;
            break;
        case 8:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
