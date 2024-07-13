#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define PRICE_PER_NIGHT 5000
#define EXTRA_SERVICES_COST 1000

// Define a structure for a room in the hotel
struct Room {
    int roomNumber; // Room number
    int isReserved; // Reservation status (1 if reserved, 0 otherwise)
    char guestName[MAX_NAME_LENGTH]; // Guest's name
    int duration; // Duration of stay in days
    int extraServices; // Extra services (1 if requested, 0 otherwise)
    int occupancy; // Number of people staying in the room
    struct tm reservationTime; // Time of reservation
    struct tm cancellationTime; // Time of cancellation
    struct Room* next; // Pointer to the next room in the list
};

// Function prototypes
struct Room* createRoom(int roomNumber);
void addRoom(struct Room** head, int roomNumber);
void makeReservation(struct Room* head, int roomNumber, const char* guestName, int duration, int extraServices, int occupancy);
void cancelReservation(struct Room* head, int roomNumber, const char* guestName);
void viewReservations(struct Room* head);
void viewRoomDetails(struct Room* head);
void freeRooms(struct Room* head);
void displayMenu();
void printHeader(const char* title);
void printLine(char ch, int length);

// Function to create a new room with given room number
struct Room* createRoom(int roomNumber) {
    struct Room* newRoom = (struct Room*)malloc(sizeof(struct Room)); // Allocate memory for a new room
    newRoom->roomNumber = roomNumber; // Set the room number
    newRoom->isReserved = 0; // Initialize reservation status to not reserved
    newRoom->guestName[0] = '\0'; // Initialize guest name to empty string
    newRoom->duration = 0; // Initialize duration to 0
    newRoom->extraServices = 0; // Initialize extra services to not requested
    newRoom->occupancy = 0; // Initialize occupancy to 0
    newRoom->reservationTime = (struct tm){0}; // Initialize reservation time to zero
    newRoom->cancellationTime = (struct tm){0}; // Initialize cancellation time to zero
    newRoom->next = NULL; // Initialize next pointer to NULL
    return newRoom; // Return the pointer to the new room
}

// Function to add a new room to the hotel
void addRoom(struct Room** head, int roomNumber) {
    struct Room* newRoom = createRoom(roomNumber); // Create a new room
    newRoom->next = *head; // Link the new room to the existing list
    *head = newRoom; // Make the new room the head of the list
    printHeader("Add Room");
    printf("Room %d added successfully.\n", roomNumber); // Print confirmation
    printLine('-', 30);
}

// Function to make a reservation
void makeReservation(struct Room* head, int roomNumber, const char* guestName, int duration, int extraServices, int occupancy) {
    struct Room* temp = head; // Temporary pointer to traverse the list
    while (temp != NULL) { // Traverse the list
        if (temp->roomNumber == roomNumber) { // Check if the room number matches
            if (!temp->isReserved) { // Check if the room is not already reserved
                temp->isReserved = 1; // Set the reservation status to reserved
                strncpy(temp->guestName, guestName, MAX_NAME_LENGTH); // Set the guest name
                temp->guestName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure the guest name is null-terminated
                temp->duration = duration; // Set the duration of stay
                temp->extraServices = extraServices; // Set the extra services request
                temp->occupancy = occupancy; // Set the number of people staying

                time_t now = time(NULL);
                temp->reservationTime = *localtime(&now); // Set the reservation time

                int totalCost = PRICE_PER_NIGHT * duration; // Calculate the total cost
                if (extraServices) {
                    totalCost += EXTRA_SERVICES_COST * duration; // Add extra services cost if requested
                }

                // Print the receipt
                printHeader("***** Receipt *****");
                printf("* Guest Name          : %s\n", guestName);
                printf("* Room Number         : %d\n", roomNumber);
                printf("* Duration of Stay    : %d days\n", duration);
                printf("* Number of People    : %d\n", occupancy);
                printf("* Price per Night     : %d PKR\n", PRICE_PER_NIGHT);
                if (extraServices) {
                    printf("* Extra Services      : %d PKR per night\n", EXTRA_SERVICES_COST);
                }
                printf("* Total Cost          : %d PKR\n", totalCost);
                printf("* Reservation Time    : %s", asctime(&temp->reservationTime));
                printLine('*', 30);
            } else {
                printHeader("Error");
                printf("Room %d is already reserved.\n", roomNumber); // Print error if room is already reserved
                printLine('-', 30);
            }
            return; // Exit the function
        }
        temp = temp->next; // Move to the next room
    }
    printHeader("Error");
    printf("Room %d not found.\n", roomNumber); // Print error if room number not found
    printLine('-', 30);
}

// Function to cancel a reservation
void cancelReservation(struct Room* head, int roomNumber, const char* guestName) {
    struct Room* temp = head; // Temporary pointer to traverse the list
    while (temp != NULL) { // Traverse the list
        if (temp->roomNumber == roomNumber) { // Check if the room number matches
            if (temp->isReserved && strcmp(temp->guestName, guestName) == 0) { // Check if the reservation matches
                temp->isReserved = 0; // Set the reservation status to not reserved
                temp->guestName[0] = '\0'; // Clear the guest name
                temp->duration = 0; // Reset the duration
                temp->extraServices = 0; // Reset the extra services
                temp->occupancy = 0; // Reset the occupancy

                time_t now = time(NULL);
                temp->cancellationTime = *localtime(&now); // Set the cancellation time

                printHeader("Cancel Reservation");
                printf("Reservation for %s in room %d canceled.\n", guestName, roomNumber); // Print confirmation
                printf("Cancellation Time: %s", asctime(&temp->cancellationTime));
                printLine('-', 30);
            } else {
                printHeader("Error");
                printf("Reservation not found for %s in room %d.\n", guestName, roomNumber); // Print error if reservation not found
                printLine('-', 30);
            }
            return; // Exit the function
        }
        temp = temp->next; // Move to the next room
    }
    printHeader("Error");
    printf("Room %d not found.\n", roomNumber); // Print error if room number not found
    printLine('-', 30);
}

// Function to view all reservations
void viewReservations(struct Room* head) {
    struct Room* temp = head; // Temporary pointer to traverse the list
    printHeader("All Reservations");
    while (temp != NULL) { // Traverse the list
        if (temp->isReserved) { // Check if the room is reserved
            printf("Room %d is reserved by %s for %d days with %d people.\n", temp->roomNumber, temp->guestName, temp->duration, temp->occupancy); // Print reservation details
            printf("Reservation Time: %s", asctime(&temp->reservationTime));
        } else {
            printf("Room %d is not reserved.\n", temp->roomNumber); // Print that the room is not reserved
        }
        temp = temp->next; // Move to the next room
    }
    printLine('-', 30);
}

// Function to view details of all rooms
void viewRoomDetails(struct Room* head) {
    struct Room* temp = head; // Temporary pointer to traverse the list
    printHeader("All Room Details");
    while (temp != NULL) { // Traverse the list
        printf("Room Number     : %d\n", temp->roomNumber);
        printf("Reservation     : %s\n", temp->isReserved ? "Reserved" : "Not Reserved");
        if (temp->isReserved) {
            printf("Guest Name      : %s\n", temp->guestName);
            printf("Duration        : %d days\n", temp->duration);
            printf("Occupancy       : %d\n", temp->occupancy);
            printf("Extra Services  : %s\n", temp->extraServices ? "Yes" : "No");
            printf("Reservation Time: %s", asctime(&temp->reservationTime));
        } else if (temp->cancellationTime.tm_year > 0) {
            printf("Last Cancellation Time: %s", asctime(&temp->cancellationTime));
        }
        printLine('-', 30);
        temp = temp->next; // Move to the next room
    }
}

// Function to free all allocated memory for the rooms
void freeRooms(struct Room* head) {
    struct Room* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Function to display the menu
void displayMenu() {
    printHeader("Hotel Management System Menu");
    printf("1. Add Room\n");
    printf("2. Make Reservation\n");
    printf("3. Cancel Reservation\n");
    printf("4. View All Reservations\n");
    printf("5. View All Room Details\n");
    printf("0. Exit\n");
    printLine('-', 30);
    printf("Enter your choice: ");
}

// Function to print a header
void printHeader(const char* title) {
    printLine('*', 30);
    printf("%s\n", title);
    printLine('*', 30);
}

// Function to print a line with a specified character and length
void printLine(char ch, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%c", ch);
    }
    printf("\n");
}

int main() {
    struct Room* head = NULL; // Initialize the head of the list
    int choice, roomNumber, duration, extraServices, occupancy;
    char guestName[MAX_NAME_LENGTH];

    do {
        displayMenu(); // Display the menu
        scanf("%d", &choice); // Get the user's choice

        switch (choice) {
            case 1:
                printf("Enter room number to add: ");
                scanf("%d", &roomNumber);
                addRoom(&head, roomNumber); // Add a new room
                break;
            case 2:
                printf("Enter room number to reserve: ");
                scanf("%d", &roomNumber);
                printf("Enter guest name: ");
                scanf(" %[^\n]", guestName); // Read a line of input for the guest name
                printf("Enter duration of stay (in days): ");
                scanf("%d", &duration);
                printf("Enter number of people staying: ");
                scanf("%d", &occupancy);
                printf("Request extra services? (1 for yes, 0 for no): ");
                scanf("%d", &extraServices);
                makeReservation(head, roomNumber, guestName, duration, extraServices, occupancy); // Make a reservation
                break;
            case 3:
                printf("Enter room number to cancel reservation: ");
                scanf("%d", &roomNumber);
                printf("Enter guest name: ");
                scanf(" %[^\n]", guestName); // Read a line of input for the guest name
                cancelReservation(head, roomNumber, guestName); // Cancel a reservation
                break;
            case 4:
                viewReservations(head); // View all reservations
                break;
            case 5:
                viewRoomDetails(head); // View details of all rooms
                break;
            case 0:
                freeRooms(head); // Free all allocated memory
                printHeader("Exiting");
                printf("Goodbye!\n");
                printLine('*', 30);
                break;
            default:
                printHeader("Invalid Choice");
                printf("Please enter a valid option.\n");
                printLine('-', 30);
                break;
        }
    } while (choice != 0);

    return 0;
}
