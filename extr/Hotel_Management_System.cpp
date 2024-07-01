#include <iostream>
#include <cstring>

using namespace std;

const int MAX_ROOMS = 100;
const int MAX_NAME_LENGTH = 50;
const int PRICE_PER_NIGHT = 5000;
const int EXTRA_SERVICES_COST = 1000;

struct Room {
    int roomNumber;
    bool isReserved;
    char guestName[MAX_NAME_LENGTH];
    int duration; // Duration of stay in days
    bool extraServices; // Extra services (food, gym, spa)
};

struct Hotel {
    Room rooms[MAX_ROOMS];
    int totalRooms;
};

void initializeHotel(Hotel* hotel);
void addRoom(Hotel* hotel, int roomNumber);
void makeReservation(Hotel* hotel, int roomNumber, const char* guestName, int duration, bool extraServices);
void cancelReservation(Hotel* hotel, int roomNumber, const char* guestName);
void viewReservations(Hotel* hotel);
void viewRoomDetails(Hotel* hotel);
void displayMenu();

int main() {
    Hotel hotel;
    initializeHotel(&hotel);

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                int roomNumber;
                cout << "Enter room number to add: ";
                cin >> roomNumber;
                addRoom(&hotel, roomNumber);
                break;
            }
            case 2: {
                int roomNumber;
                char guestName[MAX_NAME_LENGTH];
                int duration;
                bool extraServices;
                cout << "Enter room number to reserve: ";
                cin >> roomNumber;
                cout << "Enter guest name: ";
                cin.ignore();
                cin.getline(guestName, MAX_NAME_LENGTH);
                cout << "Enter duration of stay (in days): ";
                cin >> duration;
                cout << "Do you want extra services (food, gym, spa) for 1000 PKR per night? (1 for Yes, 0 for No): ";
                cin >> extraServices;
                makeReservation(&hotel, roomNumber, guestName, duration, extraServices);
                break;
            }
            case 3: {
                int roomNumber;
                char guestName[MAX_NAME_LENGTH];
                cout << "Enter room number to cancel reservation: ";
                cin >> roomNumber;
                cout << "Enter guest name: ";
                cin.ignore();
                cin.getline(guestName, MAX_NAME_LENGTH);
                cancelReservation(&hotel, roomNumber, guestName);
                break;
            }
            case 4:
                viewReservations(&hotel);
                break;
            case 5:
                viewRoomDetails(&hotel);
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}

void initializeHotel(Hotel* hotel) {
    hotel->totalRooms = 0;
}

void addRoom(Hotel* hotel, int roomNumber) {
    if (hotel->totalRooms < MAX_ROOMS) {
        hotel->rooms[hotel->totalRooms].roomNumber = roomNumber;
        hotel->rooms[hotel->totalRooms].isReserved = false;
        hotel->rooms[hotel->totalRooms].guestName[0] = '\0';
        hotel->rooms[hotel->totalRooms].duration = 0;
        hotel->rooms[hotel->totalRooms].extraServices = false;
        hotel->totalRooms++;
        cout << "Room " << roomNumber << " added successfully.\n";
    } else {
        cout << "Cannot add more rooms. Hotel is full.\n";
    }
}

void makeReservation(Hotel* hotel, int roomNumber, const char* guestName, int duration, bool extraServices) {
    for (int i = 0; i < hotel->totalRooms; i++) {
        if (hotel->rooms[i].roomNumber == roomNumber) {
            if (!hotel->rooms[i].isReserved) {
                hotel->rooms[i].isReserved = true;
                strncpy(hotel->rooms[i].guestName, guestName, MAX_NAME_LENGTH);
                hotel->rooms[i].guestName[MAX_NAME_LENGTH - 1] = '\0';
                hotel->rooms[i].duration = duration;
                hotel->rooms[i].extraServices = extraServices;

                int totalCost = PRICE_PER_NIGHT * duration;
                if (extraServices) {
                    totalCost += EXTRA_SERVICES_COST * duration;
                }
                
                cout << "\n************ Receipt ************\n";
                cout << "* Guest Name: " << guestName << "\n";
                cout << "* Room Number: " << roomNumber << "\n";
                cout << "* Duration of Stay: " << duration << " days\n";
                cout << "* Price per Night: " << PRICE_PER_NIGHT << " PKR\n";
                if (extraServices) {
                    cout << "* Extra Services (Food, Gym, Spa): " << EXTRA_SERVICES_COST << " PKR per night\n";
                }
                cout << "* Total Cost: " << totalCost << " PKR\n";
                cout << "*********************************\n";

            } else {
                cout << "Room " << roomNumber << " is already reserved.\n";
            }
            return;
        }
    }
    cout << "Room " << roomNumber << " not found.\n";
}

void cancelReservation(Hotel* hotel, int roomNumber, const char* guestName) {
    for (int i = 0; i < hotel->totalRooms; i++) {
        if (hotel->rooms[i].roomNumber == roomNumber) {
            if (hotel->rooms[i].isReserved && strcmp(hotel->rooms[i].guestName, guestName) == 0) {
                hotel->rooms[i].isReserved = false;
                hotel->rooms[i].guestName[0] = '\0';
                hotel->rooms[i].duration = 0;
                hotel->rooms[i].extraServices = false;
                cout << "Reservation for " << guestName << " in room " << roomNumber << " canceled.\n";
            } else {
                cout << "Reservation not found for " << guestName << " in room " << roomNumber << ".\n";
            }
            return;
        }
    }
    cout << "Room " << roomNumber << " not found.\n";
}

void viewReservations(Hotel* hotel) {
    for (int i = 0; i < hotel->totalRooms; i++) {
        if (hotel->rooms[i].isReserved) {
            cout << "Room " << hotel->rooms[i].roomNumber << " is reserved by " << hotel->rooms[i].guestName 
                 << " for " << hotel->rooms[i].duration << " days.\n";
        } else {
            cout << "Room " << hotel->rooms[i].roomNumber << " is not reserved.\n";
        }
    }
}

void viewRoomDetails(Hotel* hotel) {
    for (int i = 0; i < hotel->totalRooms; i++) {
        cout << "Room Number: " << hotel->rooms[i].roomNumber << "\n";
        cout << "Reservation Status: " << (hotel->rooms[i].isReserved ? "Reserved" : "Not Reserved") << "\n";
        if (hotel->rooms[i].isReserved) {
            cout << "Guest Name: " << hotel->rooms[i].guestName << "\n";
            cout << "Duration of Stay: " << hotel->rooms[i].duration << " days\n";
            cout << "Total Cost: " << PRICE_PER_NIGHT * hotel->rooms[i].duration + 
                    (hotel->rooms[i].extraServices ? EXTRA_SERVICES_COST * hotel->rooms[i].duration : 0) << " PKR\n";
        }
        cout << "-----------------------------\n";
    }
}

void displayMenu() {
    cout << "\nHotel Management System Menu:\n";
    cout << "1. Add Room\n";
    cout << "2. Make Reservation\n";
    cout << "3. Cancel Reservation\n";
    cout << "4. View All Reservations\n";
    cout << "5. View Room Details\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}
