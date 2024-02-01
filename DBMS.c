#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_NAME_LENGTH 50

// Function to clear the input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to enter a new computer part
void enterComputerPart(MYSQL *conn) {
    char name[MAX_NAME_LENGTH];
    int quantity;
    double price;

    printf("Enter the name of the computer part: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';  // Remove trailing newline

    printf("Enter the quantity: ");
    scanf("%d", &quantity);
    clearInputBuffer();

    printf("Enter the price: ");
    scanf("%lf", &price);
    clearInputBuffer();

    // Construct the INSERT query
    char query[256];
    sprintf(query, "INSERT INTO computer_parts (name, quantity, price) VALUES ('%s', %d, %.2lf)", name, quantity, price);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        exit(1);
    }

    printf("Computer part entered successfully!\n");
}

// Function to display all computer parts
void displayComputerParts(MYSQL *conn) {
    // Execute the SELECT query
    if (mysql_query(conn, "SELECT * FROM computer_parts")) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        exit(1);
    }

    printf("ID\tName\t\tQuantity\tPrice\n");
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        printf("%s\t%s\t%d\t\t%s\n", row[0], row[1], atoi(row[2]), row[3]);
    }

    // Free the result set
    mysql_free_result(result);
}

int main() {
    MYSQL *conn;

    // Initialize the connection
    conn = mysql_init(NULL);

    // Connect to the database
    if (!mysql_real_connect(conn, "localhost", "username", "password", "computer_parts_shop", 0, NULL, 0)) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        exit(1);
    }

    int choice;
    do {
        printf("\nComputer Parts Shop Database\n");
        printf("1. Enter a new computer part\n");
        printf("2. Display all computer parts\n");
        printf("3. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                enterComputerPart(conn);
                break;
            case 2:
                displayComputerParts(conn);
                break;
            case 3:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 3);

    // Close the connection
    mysql_close(conn);

    return 0;
}
