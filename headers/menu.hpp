#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int menu();
void clearScreen();
void flushInputBuffer();

int menu(vector<string> items, string title="") {
    int choice = -1;

    // Menu loop
    do {
        // Clear the screen
        clearScreen();
        cout << title << "\n\n";

        // Display the items
        for (int i = 0; i < items.size(); i++) {
            cout << (i + 1) << ": " << items[i] << "\n";
        }

        // Get user input
        cin >> choice;
        flushInputBuffer();

        // If the input is not in the valid range, reset the choice
        if (choice <= 0 || choice > items.size()) {
            choice = -1;
        }

    } while (choice == -1);

    return choice - 1;
}

void clearScreen() {
    // Clear the screen based on the platform we are on
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void flushInputBuffer() {
    // Clear input buffer
    cin.clear();
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}