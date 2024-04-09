#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

// Class to represent a customer
class Customer {
public:
    int bookingID;
    string name;
    string address;
    string phone;
    string fromDate;
    string toDate;
    float paymentAdvance;

    // Function to read customer details from file
    void readCustomerDetails(ifstream& inFile) {
        inFile >> bookingID >> name >> address >> phone >> fromDate >> toDate >> paymentAdvance;
    }

    // Function to display customer details
    void displayCustomerDetails() const {
        cout << "Booking ID: " << bookingID << endl;
        cout << "Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl;
        cout << "From Date: " << fromDate << endl;
        cout << "To Date: " << toDate << endl;
        cout << "Payment Advance: " << paymentAdvance << endl;
    }
};

// Class to represent a room
class Room {
public:
    int roomNumber;
    string type;  // Single, Double, etc.
    bool AC;
    bool occupied;

    // Function to read room details from file
    void readRoomDetails(ifstream& inFile) {
        inFile >> roomNumber >> type >> AC >> occupied;
    }

    // Function to display room details
    void displayRoomDetails() const {
        cout << "Room Number: " << roomNumber << endl;
        cout << "Type: " << type << endl;
        cout << (AC ? "AC: Yes" : "AC: No") << endl;
        cout << (occupied ? "Occupied: Yes" : "Occupied: No") << endl;
    }
};

// Global functions
bool isAdminAuthenticated() {
    const string adminPassword = "your_admin_password"; // Replace with your desired password
    string enteredPassword;

    cout << "Enter admin password: ";
    getline(cin, enteredPassword);

    return enteredPassword == adminPassword;
}

void displayMainMenu() {
    cout << "\n** Hotel Management System **\n";
    cout << "1. Check Users' Details\n";
    cout << "2. Book a Room\n";
    cout << "3. Revise Users' Information\n";
    cout << "4. Review Allotted Rooms\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    // File handling
    ifstream customerFile("customers.txt");
    ofstream customerFileOut("customers_temp.txt"); // Temporary file for updates
    ifstream roomFile("rooms.txt");
    ofstream roomFileOut("rooms_temp.txt");

    if (!customerFile.is_open() || !roomFile.is_open())
        {
        cerr << "Error opening files!" << endl;
        return 1;
    }

    // Check admin authentication
    if (!isAdminAuthenticated()) {
        cout << "Access denied." << endl;
        return 1;
    }

    int choice;
    char cont;

    do {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                // Check Users' Details
                if (customerFile.is_open()) {
                    Customer customer;
                    customerFile.clear();
                    customerFile.seekg(0);

                    while (!customerFile.eof()) {
                        customer.readCustomerDetails(customerFile);
                        customer.displayCustomerDetails();
                        cout << endl;
                    }
                } else {
                    cout << "Error accessing customer file." << endl;
                }
                break;
            case 2:
                // Book a Room
                if (roomFile.is_open()) {
                    int roomNumber;
                    bool roomFound = false;

                    Room room;
                    roomFile.clear();
                    roomFile.seekg(0);

                    cout << "Enter desired room number: ";
                    cin >> roomNumber;

                    while (!roomFile.eof()) {
                        room.readRoomDetails(roomFile);
                        if (room.roomNumber == roomNumber && !room.occupied) {
                            roomFound = true;
                            break;
                        }
                    }

                    if (roomFound) {
                        Customer customer;
                        string dateToday;

                        // Get current date
                        time_t now = time(0);
                        char buffer[80];
                        strftime(buffer, sizeof(buffer), "%d-%m-%Y", localtime(&now));
                        dateToday = buffer;

                        cout << "Enter customer details:\n";
                        cout << "Name: ";
                        getline(cin.ignore(), customer.name); // Clear newline from previous input
                        cout << "Address: ";
                        getline(cin, customer.address);
                        cout << "Phone: ";
                        getline(cin, customer.phone);

                        cout << "Enter check-in date (DD-MM-YYYY): ";
                        getline(cin, customer.fromDate);
                        cout << "Enter check-out date (DD-MM-YYYY): ";
                        getline(cin, customer.toDate);

                        cout << "Enter payment advance amount: ";
                        cin >> customer.paymentAdvance;

                        // Write customer details to temporary file
                        customerFileOut << customer.bookingID << " " << customer.name << " "
                                       << customer.address << " " << customer.phone << " "
                                       << customer.fromDate << " " << customer.toDate << " "
                                       << customer.paymentAdvance << endl;

                        // Update room occupancy in temporary file
                        roomFile.clear();
                        roomFile.seekg(0);
                        while (!roomFile.eof()) {
                            room.readRoomDetails(roomFile);
                            if (room.roomNumber == roomNumber) {
                                room.occupied = true;
                            }
                            roomFileOut << room.roomNumber << " " << room.type << " "
                                       << (room.AC ? 1 : 0) << " " << (room.occupied ? 1 : 0) << endl;
                        }

                        cout << "Room booked successfully! Booking ID: " << customer.bookingID << endl;
                    } else {
                        cout << "Room not available or error accessing files." << endl;
                    }
                } else {
                    cout << "Error accessing room file." << endl;
                }
                break;
            case 3:
                // Revise Users' Information
                if (customerFile.is_open()) {
                    int bookingID;
                    bool customerFound = false;

                    Customer customer;
                    customerFile.clear();
                    customerFile.seekg(0);

                    cout << "Enter booking ID to revise: ";
                    cin >> bookingID;

                    while (!customerFile.eof()) {
                        customer.readCustomerDetails(customerFile);
                        if (customer.bookingID == bookingID) {
                                                            customerFound = true;
                            break;
                        }
                    }

                    if (customerFound) {
                        cout << "Enter revised details (leave blank if unchanged):\n";
                        cout << "Name [" << customer.name << "]: ";
                        getline(cin.ignore(), customer.name); // Clear newline from previous input
                        cout << "Address [" << customer.address << "]: ";
                        getline(cin, customer.address);
                        cout << "Phone [" << customer.phone << "]: ";
                        getline(cin, customer.phone);

                        // Write all customer details to temporary file
                        customerFileOut << customer.bookingID << " " << customer.name << " "
                                       << customer.address << " " << customer.phone << " "
                                       << customer.fromDate << " " << customer.toDate << " "
                                       << customer.paymentAdvance << endl;

                        cout << "Customer information revised successfully!" << endl;
                    } else {
                        cout << "Customer with booking ID " << bookingID << " not found." << endl;
                    }
                } else {
                    cout << "Error accessing customer file." << endl;
                }
                break;
            case 4:
                // Review Allotted Rooms
                if (roomFile.is_open()) {
                    Room room;
                    roomFile.clear();
                    roomFile.seekg(0);

                    cout << "\nAllotted Rooms:\n";
                    while (!roomFile.eof()) {
                        room.readRoomDetails(roomFile);
                        if (room.occupied) {
                            room.displayRoomDetails();
                            cout << endl;
                        }
                    }
                    if (!roomFile.eof()) {
                        cout << "Error reading room file." << endl;
                    }
                } else {
                    cout << "Error accessing room file." << endl;
                }
                break;
            case 5:
                // Exit
                cout << "Exiting Hotel Management System..." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }

        // Close files (important for data persistence)
        customerFile.close();
        customerFileOut.close();
        roomFile.close();
        roomFileOut.close();

        // Rename temporary files to overwrite originals (data persistence)
        if (choice != 5) {
            remove("customers.txt");
            rename("customers_temp.txt", "customers.txt");
            remove("rooms.txt");
            rename("rooms_temp.txt", "rooms.txt");
        }

        cout << "\nDo you want to continue (y/n)? ";
        cin >> cont;
    } while (cont == 'y' || cont == 'Y');

    return 0;
}
