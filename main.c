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

/* --- CITY MANAGEMENT --- */
void cityManagement(void) {
    int ch;
    do {
        printf("\n--- CITY MANAGEMENT ---\n");
        printf("1. Add new city\n");
        printf("2. Rename city\n");
        printf("3. Remove city\n");
        printf("4. Display cities\n");
        printf("0. Back\n");
        ch = getInt("Enter choice: ");

        switch (ch) {
            case 1: addCity(); break;
            case 2: renameCity(); break;
            case 3: removeCity(); break;
            case 4: displayCities(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while (ch != 0);
}

void addCity(void) {
    if (cityCount >= MAX_CITIES) {
        printf("City list is full!\n");
        return;
    }
    printf("Enter city name: ");
    scanf("%49s", cities[cityCount]);
    cityCount++;
    printf("City added.\n");
}

void displayCities(void) {
    if (cityCount == 0) {
        printf("No cities added yet.\n");
        return;
    }
    printf("\nList of Cities:\n");
    for (int i = 0; i < cityCount; i++) {
        printf("%d. %s\n", i + 1, cities[i]);
    }
}

void renameCity(void) {
    if (cityCount == 0) {
        printf("No cities to rename!\n");
        return;
    }
    displayCities();
    int index = getCityIndex("Enter city number to rename: ");
    if (index < 0 || index >= cityCount) {
        printf("Invalid number!\n");
        return;
    }
    printf("Enter new name: ");
    scanf("%49s", cities[index]);
    printf("City renamed.\n");
}

void removeCity(void) {
    if (cityCount == 0) {
        printf("No cities to remove!\n");
        return;
    }
    displayCities();
    int index = getCityIndex("Enter city number to remove: ");
    if (index < 0 || index >= cityCount) {
        printf("Invalid number!\n");
        return;
    }

    for (int i = index; i < cityCount - 1; i++) {
        for (int j = 0; j < MAX_NAME_LEN; j++)
            cities[i][j] = cities[i + 1][j];
    }

    for (int i = index; i < cityCount - 1; i++) {
        for (int j = 0; j < cityCount; j++) {
            distanceMatrix[i][j] = distanceMatrix[i + 1][j];
        }
    }
    for (int j = index; j < cityCount - 1; j++) {
        for (int i = 0; i < cityCount - 1; i++) {
            distanceMatrix[i][j] = distanceMatrix[i][j + 1];
        }
    }
    cityCount--;
    printf("City removed.\n");
}

/* --- DISTANCE MANAGEMENT --- */
void distanceManagement(void) {
    int ch;
    do {
        printf("\n--- DISTANCE MANAGEMENT ---\n");
        printf("1. Enter/Edit distance\n");
        printf("2. Display distance table\n");
        printf("0. Back\n");
        ch = getInt("Enter choice: ");

        switch (ch) {
            case 1: {
                if (cityCount < 2) {
                    printf("Add at least two cities first.\n");
                    break;
                }
                displayCities();
                int s = getCityIndex("Enter source city number: ");
                int t = getCityIndex("Enter destination city number: ");
                if (s < 0 || s >= cityCount || t < 0 || t >= cityCount) {
                    printf("Invalid city index.\n");
                    break;
                }
                if (s == t) {
                    printf("Distance from city to itself = 0.\n");
                    break;
                }
                double d;
                printf("Enter distance (km): ");
                if (scanf("%lf", &d) != 1) { while (getchar()!='\n'); printf("Bad input.\n"); break; }
                distanceMatrix[s][t] = d;
                distanceMatrix[t][s] = d;
                printf("Distance updated.\n");
                break;
            }
            case 2:
                displayDistanceTable();
                break;
            case 0:
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (ch != 0);
}

/* --- NEW DELIVERY REQUEST --- */
void newDelivery(void) {
    if (cityCount < 2) {
        printf("Add cities and distances first!\n");
        return;
    }
    displayCities();
    int src = getCityIndex("Enter source city number: ");
    int dest = getCityIndex("Enter destination city number: ");
    if (src < 0 || src >= cityCount || dest < 0 || dest >= cityCount) {
        printf("Invalid city index.\n");
        return;
    }
    if (src == dest) {
        printf("Source and destination must differ.\n");
        return;
    }
    double dist = distanceMatrix[src][dest];
    if (dist <= 0) {
        printf("Distance not set between these cities.\n");
        return;
    }
    double weight;
    printf("Enter weight (kg): ");
    if (scanf("%lf", &weight) != 1) { while (getchar()!='\n'); printf("Bad input.\n"); return; }

    printf("\nSelect vehicle type:\n");
    for (int i = 0; i < NUM_VEHICLES; ++i) {
        printf("%d. %s (Capacity: %d kg, Rate: %.2f LKR/km)\n",
               i + 1, vehicles[i].name, vehicles[i].capacity, vehicles[i].rate);
    }
    int choice = getInt("Enter vehicle number: ");
    if (choice < 1 || choice > NUM_VEHICLES) {
        printf("Invalid vehicle.\n");
        return;
    }
    int v = choice - 1;
    if (weight > vehicles[v].capacity) {
        printf("Weight exceeds vehicle capacity!\n");
        return;
    }

    double baseCost = calculateCost(dist, vehicles[v].rate, weight);
    double fuelUsed = calculateFuel(dist, vehicles[v].efficiency);
    double fuelCost = fuelUsed * FUEL_PRICE;
    double operational = baseCost + fuelCost;
    double profit = baseCost * 0.25;
    double charge = operational + profit;
    double time = calculateTime(dist, vehicles[v].speed);

    if (deliveryCount >= MAX_DELIVERIES) {
        printf("Delivery list full!\n");
        return;
    }

    Delivery d;
    d.source = src; d.destination = dest; d.vehicleType = v; d.weight = weight;
    d.distance = dist; d.baseCost = baseCost; d.fuelUsed = fuelUsed; d.fuelCost = fuelCost;
    d.operationalCost = operational; d.profit = profit; d.customerCharge = charge; d.time = time;

    deliveries[deliveryCount++] = d;

    printf("\n========== DELIVERY SUMMARY ==========\n");
    printf("From: %s  To: %s\n", cities[src], cities[dest]);
    printf("Distance: %.2f km\n", dist);
    printf("Vehicle: %s\n", vehicles[v].name);
    printf("Weight: %.2f kg\n", weight);
    printf("--------------------------------------\n");
    printf("Base Cost: %.2f LKR\n", baseCost);
    printf("Fuel Used: %.2f L\n", fuelUsed);
    printf("Fuel Cost: %.2f LKR\n", fuelCost);
    printf("Operational Cost: %.2f LKR\n", operational);
    printf("Profit: %.2f LKR\n", profit);
    printf("Customer Charge: %.2f LKR\n", charge);
    printf("Estimated Time: %.2f hours\n", time);
    printf("======================================\n");
}

/* --- REPORTS --- */
void viewReports(void) {
    double totalDist = 0, totalTime = 0, totalProfit = 0, totalRevenue = 0;
    for (int i = 0; i < deliveryCount; i++) {
        totalDist += deliveries[i].distance;
        totalTime += deliveries[i].time;
        totalProfit += deliveries[i].profit;
        totalRevenue += deliveries[i].customerCharge;
    }

    printf("\n========== REPORTS ==========\n");
    printf("Total Deliveries: %d\n", deliveryCount);
    printf("Total Distance: %.2f km\n", totalDist);
    printf("Total Revenue: %.2f LKR\n", totalRevenue);
    printf("Total Profit: %.2f LKR\n", totalProfit);
    if (deliveryCount > 0)
        printf("Average Delivery Time: %.2f hours\n", totalTime / deliveryCount);
    printf("=============================\n");
}

/* --- HELPERS --- */
void displayDistanceTable(void) {
    if (cityCount == 0) {
        printf("No cities defined.\n");
        return;
    }
    printf("\n--- Distance Table (km) ---\n      ");
    for (int i = 0; i < cityCount; i++)
        printf("%10s", cities[i]);
    printf("\n");
    for (int i = 0; i < cityCount; i++) {
        printf("%-6s", cities[i]);
        for (int j = 0; j < cityCount; j++) {
            if (distanceMatrix[i][j] < 0) printf("%10s", "-");
            else printf("%10.2f", distanceMatrix[i][j]);
        }
        printf("\n");
    }
}

int getInt(const char *prompt) {
    int x;
    printf("%s", prompt);
    while (scanf("%d", &x) != 1) {
        while (getchar() != '\n');
        printf("Please enter a valid integer: ");
    }
    return x;
}

int getCityIndex(const char *prompt) {
    int num = getInt(prompt);
    return num - 1; /* user sees 1..N, we store 0..N-1 */
}

/* --- CALCULATIONS --- */
double calculateCost(double D, double R, double W) {
    return D * R * (1.0 + W / 10000.0);
}
double calculateFuel(double D, double E) {
    if (E <= 0.0) return 0.0;
    return D / E;
}
double calculateTime(double D, double S) {
    if (S <= 0.0) return 0.0;
    return D / S;
}

/* --- FILE I/O --- */
void saveRoutesToFile(void) {
    FILE *f = fopen("routes.txt", "w");
    if (!f) { printf("Error saving routes!\n"); return; }
    fprintf(f, "%d\n", cityCount);
    for (int i = 0; i < cityCount; i++)
        fprintf(f, "%s\n", cities[i]);
    for (int i = 0; i < cityCount; i++) {
        for (int j = 0; j < cityCount; j++)
            fprintf(f, "%.2f ", distanceMatrix[i][j]);
        fprintf(f, "\n");
    }
    fclose(f);
}

void loadRoutesFromFile(void) {
    FILE *f = fopen("routes.txt", "r");
    if (!f) return;
    if (fscanf(f, "%d\n", &cityCount) != 1) { fclose(f); return; }
    if (cityCount < 0 || cityCount > MAX_CITIES) { fclose(f); cityCount = 0; return; }
    for (int i = 0; i < cityCount; i++)
        fscanf(f, "%49s\n", cities[i]);
    for (int i = 0; i < cityCount; i++)
        for (int j = 0; j < cityCount; j++)
            fscanf(f, "%lf", &distanceMatrix[i][j]);
    fclose(f);
}

void saveDeliveriesToFile(void) {
    FILE *f = fopen("deliveries.txt", "w");
    if (!f) return;
    fprintf(f, "%d\n", deliveryCount);
    for (int i = 0; i < deliveryCount; i++) {
        Delivery d = deliveries[i];
        fprintf(f, "%d %d %d %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
                d.source, d.destination, d.vehicleType, d.weight, d.distance,
                d.baseCost, d.fuelUsed, d.fuelCost, d.operationalCost,
                d.profit, d.customerCharge, d.time);
    }
    fclose(f);
}

void loadDeliveriesFromFile(void) {
    FILE *f = fopen("deliveries.txt", "r");
    if (!f) return;
    if (fscanf(f, "%d\n", &deliveryCount) != 1) { fclose(f); deliveryCount = 0; return; }
    if (deliveryCount < 0 || deliveryCount > MAX_DELIVERIES) { fclose(f); deliveryCount = 0; return; }
    for (int i = 0; i < deliveryCount; i++) {
        Delivery *d = &deliveries[i];
        fscanf(f, "%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf",
               &d->source, &d->destination, &d->vehicleType, &d->weight,
               &d->distance, &d->baseCost, &d->fuelUsed, &d->fuelCost,
               &d->operationalCost, &d->profit, &d->customerCharge, &d->time);
    }
    fclose(f);
}

void saveData(void) {
    saveRoutesToFile();
    saveDeliveriesToFile();
    printf("Data saved to files.\n");
}

void loadData(void) {
    loadRoutesFromFile();
    loadDeliveriesFromFile();
    printf("Data loaded (if available).\n");
}
