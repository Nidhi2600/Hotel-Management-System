#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// Room class
class Room {
private:
    int roomNumber;
    string roomType;
    double price;
    bool booked;

public:
    Room(int number = 0, string type = "Standard",
         double cost = 0, bool status = false)
        : roomNumber(number), roomType(type),
          price(cost), booked(status) {}

    int getRoomNumber() const { return roomNumber; }
    string getRoomType() const { return roomType; }
    double getPrice() const { return price; }
    bool isBooked() const { return booked; }

    void setBooked(bool status) {
        booked = status;
    }

    void display() const {
        cout << "Room " << roomNumber
             << " | " << roomType
             << " | Rs. " << price
             << " per night | "
             << (booked ? "Booked" : "Available")
             << endl;
    }
};

// Customer class
class Customer {
private:
    int customerId;
    string name;
    string phone;

public:
    Customer(int id = 0, string customerName = "",
             string phoneNumber = "")
        : customerId(id), name(customerName),
          phone(phoneNumber) {}

    int getId() const { return customerId; }
    string getName() const { return name; }
    string getPhone() const { return phone; }

    void display() const {
        cout << "Customer ID: " << customerId
             << " | Name: " << name
             << " | Phone: " << phone << endl;
    }
};

// Booking class
class Booking {
private:
    int bookingId;
    int roomNumber;
    int customerId;
    bool active;

public:
    Booking(int id = 0, int room = 0,
            int customer = 0, bool status = true)
        : bookingId(id), roomNumber(room),
          customerId(customer), active(status) {}

    int getBookingId() const { return bookingId; }
    int getRoomNumber() const { return roomNumber; }
    int getCustomerId() const { return customerId; }
    bool isActive() const { return active; }

    void setActive(bool status) {
        active = status;
    }
};

// Hotel class
class Hotel {
private:
    vector<Room> rooms;
    vector<Customer> customers;
    vector<Booking> bookings;

    // Load room records from file
    void loadRooms() {
        rooms.clear();

        ifstream file("rooms.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string number, type, price, status;

            if (getline(ss, number, '|') &&
                getline(ss, type, '|') &&
                getline(ss, price, '|') &&
                getline(ss, status, '|')) {

                try {
                    rooms.emplace_back(
                        stoi(number), type,
                        stod(price), status == "1"
                    );
                }
                catch (...) {
                    // Skip invalid records
                }
            }
        }

        // Add default rooms if file is empty
        if (rooms.empty()) {
            rooms.emplace_back(101, "Single", 1500, false);
            rooms.emplace_back(102, "Single", 1500, false);
            rooms.emplace_back(201, "Double", 2500, false);
            rooms.emplace_back(202, "Double", 2500, false);
            rooms.emplace_back(301, "Deluxe", 4000, false);

            saveRooms();
        }
    }

    // Save room records
    void saveRooms() const {
        ofstream file("rooms.txt");

        for (const Room& room : rooms) {
            file << room.getRoomNumber() << '|'
                 << room.getRoomType() << '|'
                 << room.getPrice() << '|'
                 << (room.isBooked() ? 1 : 0)
                 << endl;
        }
    }

    // Load customer records
    void loadCustomers() {
        customers.clear();

        ifstream file("customers.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, phone;

            if (getline(ss, id, '|') &&
                getline(ss, name, '|') &&
                getline(ss, phone, '|')) {

                try {
                    customers.emplace_back(
                        stoi(id), name, phone
                    );
                }
                catch (...) {
                    // Skip invalid records
                }
            }
        }
    }

    // Save customer records
    void saveCustomers() const {
        ofstream file("customers.txt");

        for (const Customer& customer : customers) {
            file << customer.getId() << '|'
                 << customer.getName() << '|'
                 << customer.getPhone() << endl;
        }
    }

    // Load booking records
    void loadBookings() {
        bookings.clear();

        ifstream file("bookings.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string id, room, customer, active;

            if (getline(ss, id, '|') &&
                getline(ss, room, '|') &&
                getline(ss, customer, '|') &&
                getline(ss, active, '|')) {

                try {
                    bookings.emplace_back(
                        stoi(id), stoi(room),
                        stoi(customer), active == "1"
                    );
                }
                catch (...) {
                    // Skip invalid records
                }
            }
        }
    }

    // Save booking records
    void saveBookings() const {
        ofstream file("bookings.txt");

        for (const Booking& booking : bookings) {
            file << booking.getBookingId() << '|'
                 << booking.getRoomNumber() << '|'
                 << booking.getCustomerId() << '|'
                 << (booking.isActive() ? 1 : 0)
                 << endl;
        }
    }

    // Generate next customer ID
    int nextCustomerId() const {
        int maximum = 0;

        for (const Customer& customer : customers) {
            if (customer.getId() > maximum)
                maximum = customer.getId();
        }

        return maximum + 1;
    }

    // Generate next booking ID
    int nextBookingId() const {
        int maximum = 0;

        for (const Booking& booking : bookings) {
            if (booking.getBookingId() > maximum)
                maximum = booking.getBookingId();
        }

        return maximum + 1;
    }

    // Find room
    Room* findRoom(int number) {
        for (Room& room : rooms) {
            if (room.getRoomNumber() == number)
                return &room;
        }

        return nullptr;
    }

    // Find customer
    const Customer* findCustomer(int id) const {
        for (const Customer& customer : customers) {
            if (customer.getId() == id)
                return &customer;
        }

        return nullptr;
    }

    // Clear invalid input
    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    // Constructor
    Hotel() {
        loadRooms();
        loadCustomers();
        loadBookings();
    }

    // Display all rooms
    void showRooms() const {
        cout << "\n--- ROOM LIST ---\n";

        for (const Room& room : rooms) {
            room.display();
        }
    }

    // Book a room
    void bookRoom() {
        showRooms();

        int number;

        cout << "\nEnter room number to book: ";

        if (!(cin >> number)) {
            cout << "Invalid input.\n";
            clearInput();
            return;
        }

        clearInput();

        Room* room = findRoom(number);

        if (room == nullptr) {
            cout << "Room number not found.\n";
            return;
        }

        if (room->isBooked()) {
            cout << "Sorry! This room is already booked.\n";
            cout << "Double booking is not allowed.\n";
            return;
        }

        string name, phone;

        cout << "Enter customer full name: ";
        getline(cin, name);

        cout << "Enter phone number: ";
        getline(cin, phone);

        if (name.empty() || phone.empty()) {
            cout << "Details cannot be empty. Booking cancelled.\n";
            return;
        }

        int customerId = nextCustomerId();
        int bookingId = nextBookingId();

        customers.emplace_back(customerId, name, phone);

        bookings.emplace_back(
            bookingId, number, customerId, true
        );

        room->setBooked(true);

        saveRooms();
        saveCustomers();
        saveBookings();

        cout << "\nBooking successful!\n";
        cout << "Customer ID: " << customerId << endl;
        cout << "Booking ID: " << bookingId << endl;
        cout << "Room Number: " << number << endl;
    }

    // Checkout
    void checkout() {
        int number;

        cout << "Enter room number for checkout: ";

        if (!(cin >> number)) {
            cout << "Invalid input.\n";
            clearInput();
            return;
        }

        clearInput();

        Room* room = findRoom(number);

        if (room == nullptr) {
            cout << "Room number not found.\n";
            return;
        }

        if (!room->isBooked()) {
            cout << "This room is not currently booked.\n";
            return;
        }

        for (Booking& booking : bookings) {
            if (booking.getRoomNumber() == number &&
                booking.isActive()) {

                booking.setActive(false);
                break;
            }
        }

        room->setBooked(false);

        saveRooms();
        saveBookings();

        cout << "Checkout completed successfully!\n";
        cout << "Room is now available.\n";
    }

    // Search customer
    void searchCustomer() const {
        int id;

        cout << "Enter customer ID: ";

        if (!(cin >> id)) {
            cout << "Invalid input.\n";
            return;
        }

        const Customer* customer = findCustomer(id);

        if (customer != nullptr) {
            customer->display();
        }
        else {
            cout << "Customer not found.\n";
        }
    }

    // Search room
    void searchRoom() const {
        int number;

        cout << "Enter room number: ";

        if (!(cin >> number)) {
            cout << "Invalid input.\n";
            return;
        }

        for (const Room& room : rooms) {
            if (room.getRoomNumber() == number) {
                room.display();
                return;
            }
        }

        cout << "Room not found.\n";
    }

    // Display booking records
    void showBookings() const {
        cout << "\n--- BOOKING RECORDS ---\n";

        if (bookings.empty()) {
            cout << "No booking records yet.\n";
            return;
        }

        for (const Booking& booking : bookings) {
            const Customer* customer =
                findCustomer(booking.getCustomerId());

            cout << "Booking ID: " << booking.getBookingId()
                 << " | Room: " << booking.getRoomNumber()
                 << " | Customer ID: " << booking.getCustomerId()
                 << " | Customer: ";

            if (customer != nullptr)
                cout << customer->getName();
            else
                cout << "Unknown";

            cout << " | Status: "
                 << (booking.isActive() ? "Active" : "Checked out")
                 << endl;
        }
    }

    // Main menu
    void run() {
        int choice;

        do {
            cout << "\n========== HOTEL MANAGEMENT SYSTEM ==========\n";
            cout << "1. View Rooms\n";
            cout << "2. Book Room\n";
            cout << "3. Checkout\n";
            cout << "4. Search Customer\n";
            cout << "5. Search Room\n";
            cout << "6. View Booking Records\n";
            cout << "7. Exit\n";

            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cout << "Please enter a valid number.\n";
                clearInput();
                continue;
            }

            clearInput();

            switch (choice) {
                case 1:
                    showRooms();
                    break;

                case 2:
                    bookRoom();
                    break;

                case 3:
                    checkout();
                    break;

                case 4:
                    searchCustomer();
                    break;

                case 5:
                    searchRoom();
                    break;

                case 6:
                    showBookings();
                    break;

                case 7:
                    cout << "Thank you. Exiting system.\n";
                    break;

                default:
                    cout << "Invalid choice. Please select 1-7.\n";
            }

        } while (choice != 7);
    }
};


int main() {
    Hotel hotel;
    hotel.run();

    return 0;
}