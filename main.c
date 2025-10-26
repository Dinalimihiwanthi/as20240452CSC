#include <stdio.h>
#include <string.h>

#define MAX_CITIES 30
#define MAX_DELIVERIES 50
#define MAX_NAME_LEN 50
#define NUM_VEHICLES 3
#define FUEL_PRICE 310.0

/* --- Types --- */
typedef struct {
    char name[20];
    int capacity;
    double rate;       // LKR per km (base rate)
    double speed;      // km/h
    double efficiency; // km per liter
} Vehicle;

typedef struct {
    int source;
    int destination;
    int vehicleType;
    double weight;
    double distance;
    double baseCost;
    double fuelUsed;
    double fuelCost;
    double operationalCost;
    double profit;
    double customerCharge;
    double time;
} Delivery;

/* --- Globals --- */
char cities[MAX_CITIES][MAX_NAME_LEN];
int cityCount = 0;

double distanceMatrix[MAX_CITIES][MAX_CITIES];

Vehicle vehicles[NUM_VEHICLES] = {
    {"Van",   1000, 30.0, 60.0, 12.0},
    {"Truck", 5000, 40.0, 50.0, 6.0},
    {"Lorry",10000, 80.0, 45.0, 4.0}
};

Delivery deliveries[MAX_DELIVERIES];
int deliveryCount = 0;

/* --- Prototypes --- */
void menu(void);
void cityManagement(void);
void distanceManagement(void);
void newDelivery(void);
void viewReports(void);
void saveData(void);
void loadData(void);

void addCity(void);
void renameCity(void);
void removeCity(void);
void displayCities(void);
void displayDistanceTable(void);

int getInt(const char *prompt);
int getCityIndex(const char *prompt);
double calculateCost(double D, double R, double W);
double calculateFuel(double D, double E);
double calculateTime(double D, double S);

void saveRoutesToFile(void);
void loadRoutesFromFile(void);
void saveDeliveriesToFile(void);
void loadDeliveriesFromFile(void);

/* --- Main --- */
int main(void) {

    for (int i = 0; i < MAX_CITIES; ++i)
        for (int j = 0; j < MAX_CITIES; ++j)
            distanceMatrix[i][j] = (i == j) ? 0.0 : -1.0;

    loadData();
    menu();
    saveData();
    return 0;
}
/* --- MENU --- */
void menu(void) {
    int choice;
    do {
        printf("\n========== LOGISTICS MANAGEMENT SYSTEM ==========\n");
        printf("1. City Management\n");
        printf("2. Distance Management\n");
        printf("3. New Delivery Request\n");
        printf("4. Reports\n");
        printf("5. Save Data to Files\n");
        printf("6. Load Data from Files\n");
        printf("0. Exit\n");
        choice = getInt("Enter choice: ");

        switch (choice) {
            case 1: cityManagement(); break;
            case 2: distanceManagement(); break;
            case 3: newDelivery(); break;
            case 4: viewReports(); break;
            case 5: saveData(); break;
            case 6: loadData(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
}


