#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define max_username 100
#define max_pass 50
#define max_animals 100
#define animal_data_file "animal_data.txt"
#define financial_data_file "fin_data.txt"
#define milk_file "milk_file.txt"
#define sales_file "sales.txt"
#define costs_file "costs.txt"
#define feed_file "feed_management.txt"

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

typedef struct
{
    char username[max_username];
    char password[max_pass];
    int is_admin;
} User;

struct Animal
{
    int animalId;
    char breed[50];
    int age;
    char healthStatus[50];
    char vaccinationStatus[50];
    float dailyMilkYield_in_litre[31];
    float feedConsumption_in_KG;
    float feedCost;
};

User current_user;
struct Animal animals[max_animals];
int animalCount = 0;
float totalMilkSales = 0;
float totalCalfSales = 0;
float totalVeterinaryCosts = 0;
float totalLaborCosts = 0;
float totalFeedCosts = 0;
float totalEquipmentCosts = 0;

int login();
void loadFinancialData();
void saveFinancialData();
void financialReporting();
void recordCosts();
void generateMonthlyFinancialReport();
void generateYearlyFinancialReport();
void recordSales();
void main_menu();
void saveanimalData();
void loadData();
void addAnimal();
void editAnimal();
void viewAnimals();
void deleteAnimal();
void trackHealthAndVaccination();
void milkProduction();
void feedManagement();
void loadMilkProductionLog();
void loadSalesLog();
void loadCostsLog();
void loadFeedManagementLog();
void saveMilkProductionLog(int animalId, int day, float milkYield, float totalMilk);
void saveSalesLog(int type, float amount, float totalSales);
void saveCostsLog(int type, float amount, float totalCost);
void saveFeedManagementLog(int animalId, float feedConsumption, float feedCost);

int login()
{
    char username[max_username];
    char password[max_pass];

    printf("\n\t\t\t\t\t\t\t***********************************\n");
    printf("\t\t\t\t\t\t\t=== Dairy Farm Management System ===\n");
    printf("\t\t\t\t\t\t\t***********************************\n");
    printf("\t\t\t\t\t\t\tLogin\n");
    printf("\t\t\t\t\t\t\tUsername: ");
    scanf("%s", username);
    printf("\t\t\t\t\t\t\tPassword: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0)
    {
        strcpy(current_user.username, username);
        current_user.is_admin = 1;
        return 1;
    }
    return 0;
}

void saveanimalData()
{
    FILE *f_animals = fopen(animal_data_file, "w");
    if (f_animals)
    {
        for (int i = 0; i < animalCount; i++)
        {
            fprintf(f_animals, "%d,%s,%d,%s,%s\n",
                    animals[i].animalId, animals[i].breed,
                    animals[i].age, animals[i].healthStatus, animals[i].vaccinationStatus);
        }
        fclose(f_animals);
        printf("\t\tSuccessfully saved data!\n");
    }
    else
    {
        printf("Error: Could not open the required file.\n");
    }
}

void loadData()
{
    FILE *f_animals = fopen(animal_data_file, "r");
    if (f_animals)
    {
        animalCount = 0;
        char c[300];

        while (fgets(c, sizeof(c), f_animals))
        {
            if (sscanf(c, "%d,%49[^,],%d,%99[^,],%99[^\n]",
                       &animals[animalCount].animalId,
                       animals[animalCount].breed, &animals[animalCount].age,
                       animals[animalCount].healthStatus, animals[animalCount].vaccinationStatus) == 5)
            {
                animalCount++;
            }
        }
        fclose(f_animals);
        printf("Successfully loaded data!\n");
    }
    else
    {
        printf("Error: Could not open the required file\n");
    }
}

int main()
{
    loadData();
    loadFinancialData();
    loadMilkProductionLog();
    loadSalesLog();
    loadCostsLog();
    loadFeedManagementLog();

    if (login())
    {
        main_menu();
    }
    else
    {
        printf("Login failed! Please try again.\n");
    }

    return 0;
}

void main_menu()
{
    system("cls");
    int input;
    do
    {
        // system("cls");
        printf("\n\t\tMAIN MENU:\n\n");
        printf("\t\t1. Add Animal\n");
        printf("\t\t2. Edit Animal\n");
        printf("\t\t3. View Animals\n");
        printf("\t\t4. Delete Animal\n");
        printf("\t\t5. Track Health and Vaccination\n");
        printf("\t\t6. Financial Reports\n");
        printf("\t\t7. Save & Exit\n\n");
        printf("\t\tEnter your choice (1-7): ");
        scanf("%d", &input);

        switch (input)
        {
        case 1:
            addAnimal();
            saveanimalData();
            break;
        case 2:
            editAnimal();
            saveanimalData();
            break;
        case 3:
            viewAnimals();
            break;
        case 4:
            deleteAnimal();
            saveanimalData();
            break;
        case 5:
            trackHealthAndVaccination();
            break;
        case 6:
            financialReporting();
            break;
        case 7:
            system("cls");
            printf("\t\tSaving data before exit...\n");
            saveanimalData();
            saveFinancialData();
            printf("\t\tExiting...\n");
            break;
        default:
            printf("\t\tInvalid input! Please try again.\n");
        }
    } while (input != 7);
}

void addAnimal()
{
    system("cls");
    printf("Add Animal: \n");
    if (animalCount >= max_animals)
    {
        printf("Animal limit reached!\n");
        return;
    }

    struct Animal *a = &animals[animalCount];
    printf("Enter animal ID: ");
    scanf("%d", &a->animalId);

    for (int i = 0; i < animalCount; i++)
    {
        if (animals[i].animalId == a->animalId)
        {
            system("cls");
            printf("An animal with ID %d already exists!\n", a->animalId);
            return;
        }
    }

    clearInputBuffer();

    printf("Enter breed: ");
    gets(a->breed);

    printf("Enter age: ");
    scanf("%d", &a->age);

    clearInputBuffer();

    printf("Enter health status: ");
    gets(a->healthStatus);

    printf("Enter vaccination status: ");
    gets(a->vaccinationStatus);

    a->feedConsumption_in_KG = 0;
    a->feedCost = 0;
    memset(a->dailyMilkYield_in_litre, 0, sizeof(a->dailyMilkYield_in_litre));

    animalCount++;
    system("cls");
    printf("Animal added successfully!\n");
}

void editAnimal()
{
    system("cls");
    printf("Edit Animal: \n");
    int animalId;
    printf("Enter the animal ID to edit: ");
    scanf("%d", &animalId);

    for (int i = 0; i < animalCount; i++)
    {
        if (animals[i].animalId == animalId)
        {
            printf("Enter new age: ");
            scanf("%d", &animals[i].age);

            clearInputBuffer();

            printf("Enter new health status: ");
            gets(animals[i].healthStatus);

            printf("Enter new vaccination status: ");
            gets(animals[i].vaccinationStatus);

            system("cls");
            printf("Successfully updated animal details!\n");
            return;
        }
    }
    system("cls");
    printf("Animal not found!\n");
}

void viewAnimals()
{
    system("cls");
    if (animalCount == 0)
    {
        printf("There is no animals to display!\n");
        return;
    }

    printf("\n*** All Animals ***\n");
    printf("ID  | Breed                | Age (months) | Health Status       | Vaccination Status   |\n");
    printf("----+----------------------+--------------+---------------------+---------------------+\n");

    for (int i = 0; i < animalCount; i++)
    {
        struct Animal *a = &animals[i];
        printf("%-4d| %-20s | %-12d | %-20s| %-20s |\n",
               a->animalId, a->breed, a->age, a->healthStatus, a->vaccinationStatus);
    }
}

void deleteAnimal()
{
    system("cls");
    printf("Delete Animal: \n");
    int animalId;
    printf("Enter the animal ID to delete: ");
    scanf("%d", &animalId);
    clearInputBuffer();

    for (int i = 0; i < animalCount; i++)
    {
        if (animals[i].animalId == animalId)
        {
            char confirm;
            printf("Are you sure you want to delete the animal with ID %d? (y/n): ", animalId);
            scanf(" %c", &confirm);
            clearInputBuffer();

            if (confirm == 'y' || confirm == 'Y')
            {
                for (int j = i; j < animalCount - 1; j++)
                {
                    animals[j] = animals[j + 1];
                }
                animalCount--;
                system("cls");
                printf("Animal deleted successfully!\n");
            }
            else
            {
                system("cls");
                printf("Deletion canceled.\n");
            }
            return;
        }
    }
    system("cls");
    printf("Animal not found!\n");
}

void trackHealthAndVaccination()
{
    system("cls");
    printf("Track Health and Vaccination Record of Animal: \n");
    int animalId;
    printf("Enter the animal ID: ");
    scanf("%d", &animalId);

    for (int i = 0; i < animalCount; i++)
    {
        if (animals[i].animalId == animalId)
        {
            system("cls");
            printf("Animal ID: %d\n", animalId);
            printf("Health Status: %s\n", animals[i].healthStatus);
            printf("Vaccination Status: %s\n", animals[i].vaccinationStatus);
            return;
        }
    }
    system("cls");
    printf("Animal not found!\n");
}

void financialReporting()
{
    system("cls");
    int input, animalId, day;
    float amount, milkYield;

    do
    {
        printf("\n\t\tFINANCIAL MANAGEMENT:\n");
        printf("\t\t1. Record Animal Milk Production\n");
        printf("\t\t2. Record Sales\n");
        printf("\t\t3. Record Costs\n");
        printf("\t\t4. Record Feed Management\n");
        printf("\t\t5. Generate Monthly Financial Report\n");
        printf("\t\t6. Return to Main Menu\n");
        printf("\t\tEnter your choice (1-6): ");
        scanf("%d", &input);

        switch (input)
        {
        case 1:
            milkProduction();
            break;

        case 2:
            recordSales();
            break;

        case 3:
            recordCosts();
            break;

        case 4:
            feedManagement();
            break;

        case 5:
            generateMonthlyFinancialReport();
            break;

        case 6:
            system("cls");
            printf("\t\tReturning to Main Menu...\n");
            break;

        default:
            printf("\t\tInvalid input! Please try again.\n");
        }
    } while (input != 6);
}

void recordSales()
{
    system("cls");
    int input;
    float amount;

    printf("\n\t\tSALES RECORDING:\n");
    printf("\t\t1. Record Milk Sales\n");
    printf("\t\t2. Record Calf Sales\n");
    printf("\t\tEnter your choice (1-2): ");
    scanf("%d", &input);

    system("cls");

    switch (input)
    {
    case 1:
        printf("Enter total milk sales amount: ");
        scanf("%f", &amount);
        totalMilkSales += amount;
        saveSalesLog(1, amount, totalMilkSales);
        break;
    case 2:
        printf("Enter total calf sales amount: ");
        scanf("%f", &amount);
        totalCalfSales += amount;
        saveSalesLog(2, amount, totalCalfSales);
        break;
    default:
        system("cls");
        printf("Invalid input!\n");
        return;
    }
    system("cls");
    printf("Successfully recorded sales!\n");
}

void recordCosts()
{
    system("cls");
    int input;
    float amount;

    printf("\n\t\tCOSTS RECORDING:\n");
    printf("\t\t1. Veterinary Costs\n");
    printf("\t\t2. Labor Costs\n");
    printf("\t\t3. Feed Costs\n");
    printf("\t\t4. Equipment Costs\n");
    printf("\t\tEnter your choice (1-4): ");
    scanf("%d", &input);

    system("cls");

    switch (input)
    {
    case 1:
        printf("Enter veterinary costs: ");
        scanf("%f", &amount);
        totalVeterinaryCosts += amount;
        saveCostsLog(1, amount, totalVeterinaryCosts);
        break;
    case 2:
        printf("Enter labor costs: ");
        scanf("%f", &amount);
        totalLaborCosts += amount;
        saveCostsLog(2, amount, totalLaborCosts);
        break;
    case 3:
        printf("Enter feed costs: ");
        scanf("%f", &amount);
        totalFeedCosts += amount;
        saveCostsLog(3, amount, totalFeedCosts);
        break;
    case 4:
        printf("Enter equipment costs: ");
        scanf("%f", &amount);
        totalEquipmentCosts += amount;
        saveCostsLog(4, amount, totalEquipmentCosts);
        break;
    default:
        system("cls");
        printf("Invalid input!\n");
        return;
    }
    system("cls");
    printf("Successfully recorded costs!\n");
}

void generateMonthlyFinancialReport()
{
    system("cls");
    float totalSales = totalMilkSales + totalCalfSales;
    float totalCosts = totalVeterinaryCosts + totalLaborCosts +
                       totalFeedCosts + totalEquipmentCosts;
    float netProfit = totalSales - totalCosts;

    printf("\n\t\t=== MONTHLY FINANCIAL REPORT ===\n");
    printf("\t\tMilk Sales:         $%.2f\n", totalMilkSales);
    printf("\t\tCalf Sales:         $%.2f\n", totalCalfSales);
    printf("\t\tTotal Sales:        $%.2f\n\n", totalSales);

    printf("\t\tVeterinary Costs:   $%.2f\n", totalVeterinaryCosts);
    printf("\t\tLabor Costs:        $%.2f\n", totalLaborCosts);
    printf("\t\tFeed Costs:         $%.2f\n", totalFeedCosts);
    printf("\t\tEquipment Costs:    $%.2f\n", totalEquipmentCosts);
    printf("\t\tTotal Costs:        $%.2f\n\n", totalCosts);

    printf("\t\tNet Profit:         $%.2f\n", netProfit);
}

void saveFinancialData()
{
    FILE *file = fopen(financial_data_file, "w");
    if (file == NULL)
    {
        printf("Error: Could not open financial data file.\n");
        return;
    }

    fprintf(file, "Total Milk Sales: %.2f\nTotal Calf Sales: %.2f\nTotal Veterinary Costs: %.2f\nTotal Labor Costs: %.2f\nTotal Feed Costs: %.2f\nTotal Equipment Costs: %.2f\n",
            totalMilkSales, totalCalfSales,
            totalVeterinaryCosts, totalLaborCosts,
            totalFeedCosts, totalEquipmentCosts);

    fclose(file);
    printf("Successfully saved financial data!\n");
}

void loadFinancialData()
{
    FILE *file = fopen(financial_data_file, "r");
    if (file == NULL)
    {

        printf("No previous financial data found.\n");
        return;
    }

    if (fscanf(file, "%f\n%f\n%f\n%f\n%f\n%f\n",
               &totalMilkSales, &totalCalfSales,
               &totalVeterinaryCosts, &totalLaborCosts,
               &totalFeedCosts, &totalEquipmentCosts) != 6)
    {
        printf("Error: Incomplete financial data.\n");
    }

    fclose(file);
    printf("Successfully loaded financial data!\n");
}

void milkProduction()
{
    system("cls");
    printf("Record Animal Milk Production: \n");
    int animalId, day;
    float milkYield;
    float totalMilk = 0;

    printf("Enter the animal ID: ");
    scanf("%d", &animalId);

    for (int i = 0; i < animalCount; i++)
    {
        if (animals[i].animalId == animalId)
        {
            printf("Enter the day of the month (1-31): ");
            scanf("%d", &day);

            if (day < 1 || day > 31)
            {
                system("cls");
                printf("Invalid day!\n");
                break;
            }

            printf("Enter milk yield for day %d: ", day);
            scanf("%f", &milkYield);

            animals[i].dailyMilkYield_in_litre[day - 1] += milkYield;

            for (int j = 0; j < 31; j++)
            {
                totalMilk += animals[i].dailyMilkYield_in_litre[j];
            }

            saveMilkProductionLog(animalId, day, milkYield, totalMilk);

            system("cls");
            printf("Total milk production for the month for animal ID %d is : %.2f liters\n", animalId, totalMilk);
            printf("Animal milk production recorded successfully!");
            break;
        }
    }
}

void feedManagement()
{
    system("cls");
    printf("Record Feed Management: \n");
    int animalId;

    printf("Enter the animal ID: ");
    scanf("%d", &animalId);

    for (int i = 0; i < animalCount; i++)
    {
        if (animals[i].animalId == animalId)
        {
            printf("Enter feed consumption (kg): ");
            scanf("%f", &animals[i].feedConsumption_in_KG);
            printf("Enter feed cost: ");
            scanf("%f", &animals[i].feedCost);

            saveFeedManagementLog(animalId, animals[i].feedConsumption_in_KG, animals[i].feedCost);

            system("cls");
            printf("Feed consumption and cost recorded successfully!\n");
            break;
        }
    }
}

void saveMilkProductionLog(int animalId, int day, float milkYield, float totalMilk)
{
    FILE *milk_log = fopen(milk_file, "a");
    if (milk_log == NULL)
    {
        printf("Error: Could not open milk production log file.\n");
        return;
    }

    fprintf(milk_log, "Animal ID: %d, Day: %d, Milk Yield: %.2f liters, Total Month Milk: %.2f liters\n",
            animalId, day, milkYield, totalMilk);
    fclose(milk_log);
}

void saveSalesLog(int type, float amount, float totalSales)
{
    FILE *sales_log = fopen(sales_file, "a");
    if (sales_log == NULL)
    {
        printf("Error: Could not open sales log file.\n");
        return;
    }

    if (type == 1)
        fprintf(sales_log, "Milk Sales: $%.2f, Total Milk Sales: $%.2f\n", amount, totalSales);
    else if (type == 2)
        fprintf(sales_log, "Calf Sales: $%.2f, Total Calf Sales: $%.2f\n", amount, totalSales);

    fclose(sales_log);
}

void saveCostsLog(int type, float amount, float totalCost)
{
    FILE *costs_log = fopen(costs_file, "a");
    if (costs_log == NULL)
    {
        printf("Error: Could not open costs log file.\n");
        return;
    }

    switch (type)
    {
    case 1:
        fprintf(costs_log, "Veterinary Costs: $%.2f, Total Veterinary Costs: $%.2f\n", amount, totalCost);
        break;
    case 2:
        fprintf(costs_log, "Labor Costs: $%.2f, Total Labor Costs: $%.2f\n", amount, totalCost);
        break;
    case 3:
        fprintf(costs_log, "Feed Costs: $%.2f, Total Feed Costs: $%.2f\n", amount, totalCost);
        break;
    case 4:
        fprintf(costs_log, "Equipment Costs: $%.2f, Total Equipment Costs: $%.2f\n", amount, totalCost);
        break;
    }

    fclose(costs_log);
}

void saveFeedManagementLog(int animalId, float feedConsumption, float feedCost)
{
    FILE *feed_log = fopen(feed_file, "a");
    if (feed_log == NULL)
    {
        printf("Error: Could not open feed management log file.\n");
        return;
    }

    fprintf(feed_log, "Animal ID: %d, Feed Consumption: %.2f kg, Feed Cost: $%.2f\n",
            animalId, feedConsumption, feedCost);
    fclose(feed_log);
}

void loadMilkProductionLog()
{
    FILE *milk_log = fopen(milk_file, "r");
    if (milk_log == NULL)
    {
        printf("No previous milk production record found.\n");
        return;
    }

    char c[200];
    while (fgets(c, sizeof(c), milk_log))
    {
        int animalId, day;
        float milkYield, totalMilk;

        if (sscanf(c, "Animal ID: %d, Day: %d, Milk Yield: %f liters, Total Month Milk: %f liters",
                   &animalId, &day, &milkYield, &totalMilk) == 4)
        {
            for (int i = 0; i < animalCount; i++)
            {
                if (animals[i].animalId == animalId)
                {
                    animals[i].dailyMilkYield_in_litre[day - 1] = milkYield;
                    break;
                }
            }
        }
    }

    fclose(milk_log);
    printf("Successfully loaded milk production data!\n");
}

void loadSalesLog()
{
    FILE *sales_log = fopen(sales_file, "r");
    if (sales_log == NULL)
    {
        printf("No previous sales data found.\n");
        return;
    }

    char c[200];
    while (fgets(c, sizeof(c), sales_log))
    {
        float amount, totalSales;
        char type[20];

        if (sscanf(c, "%s Sales: $%f, Total %s Sales: $%f",
                   type, &amount, type, &totalSales) == 4)
        {
            if (strcmp(type, "Milk") == 0)
                totalMilkSales = totalSales;
            else if (strcmp(type, "Calf") == 0)
                totalCalfSales = totalSales;
        }
    }

    fclose(sales_log);
    printf("Successfully loaded sales data!\n");
}

void loadCostsLog()
{
    FILE *costs_log = fopen(costs_file, "r");
    if (costs_log == NULL)
    {
        printf("No previous costs data found.\n");
        return;
    }

    char c[200];
    while (fgets(c, sizeof(c), costs_log))
    {
        float amount, totalCost;
        char type[20];

        if (sscanf(c, "%s Costs: $%f, Total %s Costs: $%f",
                   type, &amount, type, &totalCost) == 4)
        {
            if (strcmp(type, "Veterinary") == 0)
                totalVeterinaryCosts = totalCost;
            else if (strcmp(type, "Labor") == 0)
                totalLaborCosts = totalCost;
            else if (strcmp(type, "Feed") == 0)
                totalFeedCosts = totalCost;
            else if (strcmp(type, "Equipment") == 0)
                totalEquipmentCosts = totalCost;
        }
    }

    fclose(costs_log);
    printf("Successfully loaded costs data!\n");
}

void loadFeedManagementLog()
{
    FILE *feed_log = fopen(feed_file, "r");
    if (feed_log == NULL)
    {
        printf("No previous feed management data found.\n");
        return;
    }

    char c[200];
    while (fgets(c, sizeof(c), feed_log))
    {
        int animalId;
        float feedConsumption, feedCost;

        if (sscanf(c, "Animal ID: %d, Feed Consumption: %f kg, Feed Cost: $%f",
                   &animalId, &feedConsumption, &feedCost) == 3)
        {
            for (int i = 0; i < animalCount; i++)
            {
                if (animals[i].animalId == animalId)
                {
                    animals[i].feedConsumption_in_KG = feedConsumption;
                    animals[i].feedCost = feedCost;
                    break;
                }
            }
        }
    }

    fclose(feed_log);
    printf("Successfully loaded feed management data!\n");
}
