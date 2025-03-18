#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void clearScreen() {
    system("cls || clear");
}

//headers
void mainMenuHeader() {
    printf("=== NOTES APPLICATION ===\n");
    printf("-------------------------\n");
    printf("1 - Add a new note\n");
    printf("2 - View notes\n");
    printf("3 - Edit a note\n");
    printf("4 - Delete a note\n");
    printf("0 - Exit\n");
    printf("-------------------------\n");
}

void addNoteHeader() {
    printf("=== ADD A NEW NOTE ===\n");
    printf("----------------------\n");
}

void viewNotesHeader() {
    printf("=== VIEW NOTES ===\n");
    printf("------------------\n");
}

void editNoteHeader() {
    printf("=== EDIT NOTE ===\n");
    printf("-----------------\n");
}

void deleteNoteHeader() {
    printf("=== DELETE NOTE ===\n");
    printf("-------------------\n");
}

// usability functions
void addNote() {
    clearScreen();
    addNoteHeader();

    printf("Not implemented yet. Please press enter to continue...");
    getchar();
    clearScreen();
}

void viewNotes() {
    clearScreen();
    viewNotesHeader();

    printf("Not implemented yet. Please press enter to continue...");
    getchar();
    clearScreen();
}

void editNote() {
    clearScreen();
    editNoteHeader();

    printf("Not implemented yet. Please press enter to continue...");
    getchar();
    clearScreen();
}

void deleteNote() {
    clearScreen();
    deleteNoteHeader();

    printf("Not implemented yet. Please press enter to continue...");
    getchar();
    clearScreen();
}
//menu options
void menu(int option) {
    switch (option) {
    case 1:
        addNote();
        break;
    case 2:
        viewNotes();
        break;
    case 3:
        editNote();
        break;
    case 4:
        deleteNote();
        break;
    default:
        printf("Exiting application...\n");
    }
}


int main() {
    int option;

    do {
        mainMenuHeader();
        printf("Enter option: ");
        scanf("%d", &option);
        getchar();

        if (option >= 0 && option <= 4) {
            menu(option);
        }
        else {
            clearScreen();
            printf("Invalid option! Please try again.\n\n");
        }
    } while (option != 0);
}