#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float price;
    int stock;
} Product;

Product* inventory = NULL;
int numProducts = 0;

void loadInventory() {
    FILE* file = fopen("inventory.csv", "r");
    if (file == NULL) {
        printf("Error opening file! Creating new inventory.\n");
        return;
    }

    char line[1000];
    // Skip header line
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Inventory is empty or invalid!\n");
        fclose(file);
        return;
    }

    // Count number of products
    while (fgets(line, sizeof(line), file)) {
        numProducts++;
    }

    rewind(file);

    // Allocate memory
    inventory = (Product*)malloc(numProducts * sizeof(Product));
    if (inventory == NULL) {
        printf("Memory allocation error!\n");
        fclose(file);
        return;
    }

    int i = 0;
    fgets(line, sizeof(line), file); // Skip header
    while (i < numProducts && fscanf(file, "%d,%49[^,],%f,%d\n", &inventory[i].id, inventory[i].name,
            &inventory[i].price, &inventory[i].stock) == 4) {
        i++;
    }

    if (i != numProducts) {
        printf("Warning: Only %d products loaded, expected %d.\n", i, numProducts);
        numProducts = i;
    }

    fclose(file);
    printf("Loaded %d products from inventory.csv.\n", numProducts);
}

void displayInventory() {
    if (numProducts == 0 || inventory == NULL) {
        printf("No products in inventory.\n");
        return;
    }

    printf("-----------------------------------------------------------------\n");
    printf("|%-10s|%-25s|%-10s|%-15s|\n", "Product ID", "Product Name", "Price ($)", "Stock Quantity");
    printf("-----------------------------------------------------------------\n");

    for (int i = 0; i < numProducts; i++) {
        printf("|%-10d|%-25s|%-10.2f|%-15d|\n", inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].stock);
    }
}

void addProducts() {
    numProducts++;
    Product* temp = (Product*)realloc(inventory, numProducts * sizeof(Product));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        numProducts--;
        return;
    }
    inventory = temp;

    // Assign ID automatically
    inventory[numProducts - 1].id = (numProducts == 1) ? 101 : inventory[numProducts - 2].id + 1;

    printf("Enter Product Name: ");
    fgets(inventory[numProducts - 1].name, 50, stdin);
    int name_len = strlen(inventory[numProducts - 1].name);
    if (inventory[numProducts - 1].name[name_len - 1] == '\n') {
        inventory[numProducts - 1].name[name_len - 1] = '\0';
    }

    // Validate price
    do {
        printf("Enter the price (positive number): ");
        if (scanf(" %f", &inventory[numProducts - 1].price) != 1 || inventory[numProducts - 1].price < 0) {
            printf("Invalid price! Please enter a positive number.\n");
            while (getchar() != '\n'); // Clear input buffer
        } else {
            break;
        }
    } while (1);

    // Validate stock quantity
    do {
        printf("Enter stock quantity (non-negative integer): ");
        if (scanf(" %d", &inventory[numProducts - 1].stock) != 1 || inventory[numProducts - 1].stock < 0) {
            printf("Invalid stock quantity! Please enter a non-negative integer.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    } while (1);

    while (getchar() != '\n'); // Clear input buffer
    printf("Product added successfully!\n");
}

void updateProductDetail() {
    if (numProducts == 0 || inventory == NULL) {
        printf("No products to update. Inventory is empty!\n");
        return;
    }

    int ProductID;
    printf("Enter the Product ID to update: ");
    if (scanf(" %d", &ProductID) != 1) {
        printf("Invalid input for Product ID!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int found = 0, position = 0;
    for (int i = 0; i < numProducts; i++) {
        if (inventory[i].id == ProductID) {
            found = 1;
            position = i;
            break;
        }
    }

    if (!found) {
        printf("Product with ID %d not found!\n", ProductID);
        return;
    }

    printf("Enter new Product name: ");
    fgets(inventory[position].name, 50, stdin);
    int name_len = strlen(inventory[position].name);
    if (inventory[position].name[name_len - 1] == '\n') {
        inventory[position].name[name_len - 1] = '\0';
    }

    // Validate price
    do {
        printf("Enter new price (positive number): ");
        if (scanf(" %f", &inventory[position].price) != 1 || inventory[position].price < 0) {
            printf("Invalid price! Please enter a positive number.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    } while (1);

    // Validate stock quantity
    do {
        printf("Enter new stock quantity (non-negative integer): ");
        if (scanf(" %d", &inventory[position].stock) != 1 || inventory[position].stock < 0) {
            printf("Invalid stock quantity! Please enter a non-negative integer.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    } while (1);

    while (getchar() != '\n');
    printf("Product with ID %d updated successfully!\n", ProductID);
}

void deleteProduct() {
    if (numProducts == 0 || inventory == NULL) {
        printf("No products to delete. Inventory is empty!\n");
        return;
    }

    int ProductID;
    printf("Enter the Product ID to delete: ");
    if (scanf(" %d", &ProductID) != 1) {
        printf("Invalid input for Product ID!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int found = 0, position = 0;
    for (int i = 0; i < numProducts; i++) {
        if (inventory[i].id == ProductID) {
            found = 1;
            position = i;
            break;
        }
    }

    if (!found) {
        printf("Product with ID %d not found!\n", ProductID);
        return;
    }

    printf("Are you sure you want to delete product ID %d? (1 for Yes, 0 for No): ", ProductID);
    int confirm;
    if (scanf(" %d", &confirm) != 1) {
        printf("Invalid input! Deletion canceled.\n");
        while (getchar() != '\n');
        return;
    }

    if (confirm != 1) {
        printf("Deletion canceled!\n");
        return;
    }

    for (int i = position; i < numProducts - 1; i++) {
        inventory[i] = inventory[i + 1];
    }

    numProducts--;
    Product* temp = (Product*)realloc(inventory, numProducts * sizeof(Product));
    if (temp == NULL && numProducts > 0) {
        printf("Memory reallocation failed!\n");
        return;
    }
    inventory = temp;

    printf("Product with ID %d deleted successfully!\n", ProductID);
}

void searchProduct() {
    if (numProducts == 0 || inventory == NULL) {
        printf("No products to search. Inventory is empty!\n");
        return;
    }

    int choice;
    printf("Search by:\n");
    printf("1. Product Name\n");
    printf("2. Price\n");
    printf("Enter your choice: ");
    if (scanf(" %d", &choice) != 1) {
        printf("Invalid choice!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    char searchName[50];
    float searchPrice;
    int found = 0;

    switch (choice) {
    case 1:
        printf("Enter Product Name to search: ");
        fgets(searchName, 50, stdin);
        int name_len = strlen(searchName);
        if (searchName[name_len - 1] == '\n') {
            searchName[name_len - 1] = '\0';
        }

        for (int i = 0; i < numProducts; i++) {
            if (strcmp(inventory[i].name, searchName) == 0) {
                found = 1;
                printf("-----------------------------------------------------------------\n");
                printf("|%-10s|%-25s|%-10s|%-15s|\n", "Product ID", "Product Name", "Price ($)", "Stock Quantity");
                printf("-----------------------------------------------------------------\n");
                printf("|%-10d|%-25s|%-10.2f|%-15d|\n", inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].stock);
            }
        }

        if (!found) {
            printf("No products found with that name.\n");
        }
        break;

    case 2:
        printf("Enter Price to search: ");
        if (scanf(" %f", &searchPrice) != 1 || searchPrice < 0) {
            printf("Invalid price!\n");
            while (getchar() != '\n');
            return;
        }

        for (int i = 0; i < numProducts; i++) {
            if (inventory[i].price == searchPrice) {
                found = 1;
                printf("-----------------------------------------------------------------\n");
                printf("|%-10s|%-25s|%-10s|%-15s|\n", "Product ID", "Product Name", "Price ($)", "Stock Quantity");
                printf("-----------------------------------------------------------------\n");
                printf("|%-10d|%-25s|%-10.2f|%-15d|\n", inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].stock);
            }
        }

        if (!found) {
            printf("No products found with that price.\n");
        }
        break;

    default:
        printf("Invalid choice!\n");
        break;
    }
}

void sortInventory() {
    if (numProducts == 0 || inventory == NULL) {
        printf("No products to sort. Inventory is empty!\n");
        return;
    }

    int choice;
    printf("Sort by:\n");
    printf("1. Product Name\n");
    printf("2. Price\n");
    printf("3. Stock Quantity\n");
    printf("Enter your choice: ");
    if (scanf(" %d", &choice) != 1) {
        printf("Invalid choice!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int order;
    printf("1. Ascending\n");
    printf("2. Descending\n");
    printf("Enter your choice: ");
    if (scanf(" %d", &order) != 1 || (order != 1 && order != 2)) {
        printf("Invalid order! Please choose 1 or 2.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    for (int i = 0; i < numProducts - 1; i++) {
        for (int j = 0; j < numProducts - i - 1; j++) {
            int swapNeeded = 0;
            if (choice == 1) { // Sort by name
                int compare = strcmp(inventory[j].name, inventory[j + 1].name);
                swapNeeded = (order == 1) ? (compare > 0) : (compare < 0);
            } else if (choice == 2) { // Sort by price
                swapNeeded = (order == 1) ? (inventory[j].price > inventory[j + 1].price) :
                                          (inventory[j].price < inventory[j + 1].price);
            } else if (choice == 3) { // Sort by stock
                swapNeeded = (order == 1) ? (inventory[j].stock > inventory[j + 1].stock) :
                                          (inventory[j].stock < inventory[j + 1].stock);
            }

            if (swapNeeded) {
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }

    printf("Sorted Inventory:\n");
    displayInventory();
}

void Save_Exit() {
    FILE* file = fopen("inventory.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        if (inventory != NULL) {
            free(inventory);
            inventory = NULL;
        }
        return;
    }

    fprintf(file, "Product_ID,Product_Name,Price,Stock_Quantity\n");
    for (int i = 0; i < numProducts; i++) {
        fprintf(file, "%d,%s,%.2f,%d\n", inventory[i].id, inventory[i].name,
                inventory[i].price, inventory[i].stock);
    }

    fclose(file);
    printf("All changes saved to inventory.csv.\n");
    if (inventory != NULL) {
        free(inventory);
        inventory = NULL;
        printf("Memory has been freed.\n");
    }
}

int main() {
    loadInventory();
    
    int choice;
    while (1) {
        printf("\nInventory Management System\n");
        printf("1. Display Products\n");
        printf("2. Add Product\n");
        printf("3. Update Product\n");
        printf("4. Delete Product\n");
        printf("5. Search Product\n");
        printf("6. Sort Inventory\n");
        printf("7. Save and Exit\n");
        printf("Enter your choice: ");
        if (scanf(" %d", &choice) != 1) {
            printf("Invalid choice! Please try again.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        printf("\n");

        switch (choice) {
        case 1:
            displayInventory();
            break;
        case 2:
            addProducts();
            break;
        case 3:
            updateProductDetail();
            break;
        case 4:
            deleteProduct();
            break;
        case 5:
            searchProduct();
            break;
        case 6:
            sortInventory();
            break;
        case 7:
            Save_Exit();
            return 0; // Exit program
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}