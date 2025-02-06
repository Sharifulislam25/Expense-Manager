#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <vector>
using namespace std;

struct ExpenseItem {
    char date[6];
    char item[50];
    double cost;
};

void addExpenses() {
    ExpenseItem expense;
    char continueAdding = 'y';

    cout << "Enter date (DD-MM): ";
    cin >> expense.date;

    ofstream file("expenses.txt", ios::app);
    while (continueAdding == 'y') {
        cout << "Enter item name: ";
        cin >> expense.item; // No getline, only cin
        cout << "Enter item cost: ";
        cin >> expense.cost;
        file << expense.date << "," << expense.item << "," << expense.cost << "\n";
        cout << "Add another item? (y/n): ";
        cin >> continueAdding;
    }
    file.close();
    cout << "Expenses saved.\n";
}

void calculateStats() {
    ifstream file("expenses.txt");
    if (!file.is_open()) {
        cout << "No data.\n";
        return;
    }

    char line[200], date[6], item[50];
    double cost, total = 0;
    int dayCount = 0;
    char lastDate[6] = "";

    while (file.getline(line, 200)) {
        int i = 0, j = 0;

        // Parse date
        while (line[i] != ',' && line[i] != '\0') date[j++] = line[i++];
        date[j] = '\0';
        i++;

        // Parse item
        j = 0;
        while (line[i] != ',' && line[i] != '\0') item[j++] = line[i++];
        item[j] = '\0';
        i++;

        // Parse cost
        char costStr[20];
        j = 0;
        while (line[i] != '\0') costStr[j++] = line[i++];
        costStr[j] = '\0';
        cost = atof(costStr);

        if (strcmp(lastDate, date) != 0) {
            strcpy(lastDate, date);
            dayCount++;
        }
        total += cost;
    }
    file.close();

    cout << "Total: " << fixed << setprecision(2) << total << "\n";
    if (dayCount > 0) {
        cout << "Average: " << fixed << setprecision(2) << total / dayCount << "\n";
    }
}

vector<ExpenseItem> loadExpenses() {
    vector<ExpenseItem> expenses;
    ifstream file("expenses.txt");
    if (!file.is_open()) return expenses;

    char line[200];
    while (file.getline(line, 200)) {
        ExpenseItem expense;
        int i = 0, j = 0;

        while (line[i] != ',' && line[i] != '\0') expense.date[j++] = line[i++];
        expense.date[j] = '\0';
        i++;
        j = 0;

        while (line[i] != ',' && line[i] != '\0') expense.item[j++] = line[i++];
        expense.item[j] = '\0';
        i++;
        j = 0;

        char costStr[20];
        while (line[i] != '\0') costStr[j++] = line[i++];
        costStr[j] = '\0';
        expense.cost = atof(costStr);

        expenses.push_back(expense);
    }
    file.close();
    return expenses;
}

void searchExpense() {
    string searchItem;
    cout << "Enter item to search: ";
    cin >> searchItem;

    vector<ExpenseItem> expenses = loadExpenses();
    bool found = false;

    for (const auto& expense : expenses) {
        if (searchItem == expense.item) {
            cout << "Found: " << expense.date << " " << expense.item << " " << expense.cost << "\n";
            found = true;
        }
    }

    if (!found) cout << "Item not found.\n";
}

void sortExpenses() {
    vector<ExpenseItem> expenses = loadExpenses();
    if (expenses.empty()) {
        cout << "No data to sort.\n";
        return;
    }

    for (size_t i = 1; i < expenses.size(); i++) {
        ExpenseItem key = expenses[i];
        int j = i - 1;
        while (j >= 0 && expenses[j].cost > key.cost) {
            expenses[j + 1] = expenses[j];
            j--;
        }
        expenses[j + 1] = key;
    }

    cout << "Sorted Expenses by Cost:\n";
    for (const auto& expense : expenses) {
        cout << expense.date << " " << expense.item << " " << fixed << setprecision(2) << expense.cost << "\n";
    }
}

int main() {
    int choice;

    do {
        cout << "\n1. Add Expenses\n";
        cout << "2. View Stats\n";
        cout << "3. Search\n";
        cout << "4. Sort\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addExpenses();
                break;
            case 2:
                calculateStats();
                break;
            case 3:
                searchExpense();
                break;
            case 4:
                sortExpenses();
                break;
            case 5:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 5);

    return 0;
}
