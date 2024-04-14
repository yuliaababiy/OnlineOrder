#include <iostream>
#include <queue>
#include <string>
#include <ctime>

using namespace std;

// Structure to represent a product
struct Product {
    string name;
    double price;
    int quantity;
};

// Structure to represent an order
struct Order {
    int orderNumber;
    time_t orderDate;
    string status;
    double totalAmount;
    queue<Product> products;
};

// Function to add a product to the order
void addProduct(Order& order) {
    Product product;
    cout << "Enter product name: ";
    cin >> product.name;
    cout << "Enter product price: ";
    cin >> product.price;
    cout << "Enter product quantity: ";
    cin >> product.quantity;
    order.products.push(product);
}

// Function to select a product from the order
void selectProduct(Order& order) {
    int productIndex;
    cout << "Enter the index of the product you want to select: ";
    cin >> productIndex;

    // Check if the index is valid
    if (productIndex >= 0 && productIndex < order.products.size()) {
        // Display the selected product
        cout << "Selected product: " << order.products.front().name << endl;
    }
    else {
        cout << "Invalid product index." << endl;
    }
}

// Function to delete a product from the order
void deleteProduct(Order& order) {
    order.products.pop();
    cout << "Product deleted successfully." << endl;
}

int main() {
    Order order;
    order.orderNumber = 1;
    order.orderDate = time(nullptr);
    order.status = "Pending";
    order.totalAmount = 0.0;

    int choice;
    do {
        cout << "\nOnline Order System\n";
        cout << "1. Add Product\n";
        cout << "2. Select Product\n";
        cout << "3. Delete Product\n";
        cout << "4. View Order Details\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addProduct(order);
            break;
        case 2:
            selectProduct(order);
            break;
        case 3:
            deleteProduct(order);
            break;
        case 4:
            cout << "Order Number: " << order.orderNumber << endl;
            cout << "Order Date: " << ctime(&order.orderDate);
            cout << "Order Status: " << order.status << endl;
            cout << "Total Amount: " << order.totalAmount << endl;
            cout << "Products in Order:\n";
            {
                queue<Product> temp = order.products;
                int index = 1;
                while (!temp.empty()) {
                    cout << index << ". " << temp.front().name << " - $" << temp.front().price << " - Quantity: " << temp.front().quantity << endl;
                    temp.pop();
                    index++;
                }
            }
            break;
        case 5:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Please try again.\n";
        }
    } while (choice != 5);

    return 0;
    //+якусь свою особливу функцію!
}
