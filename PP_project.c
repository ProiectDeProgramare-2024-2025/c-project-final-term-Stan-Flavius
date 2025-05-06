#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h> 
#include <ctype.h>

#define MAX_NOTES 100
#define MAX_TITLE_LENGTH 50
#define MAX_CONTENT_LENGTH 2000
#define NOTES_DIR "notes"
#define MAX_PATH_LENGTH 260

#define COLOR_RESET 7
#define COLOR_TITLE 11
#define COLOR_MENU 14
#define COLOR_HEADER 10
#define COLOR_NOTE_TITLE 13
#define COLOR_ERROR 12
#define COLOR_SUCCESS 10
#define COLOR_PROMPT 15
#define COLOR_FILENAME 9

typedef struct {
    char title[MAX_TITLE_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    char filename[MAX_PATH_LENGTH];
} Note;

Note notes[MAX_NOTES];
int noteCount = 0;
HANDLE hConsole;

void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void resetColor() {
    SetConsoleTextAttribute(hConsole, COLOR_RESET);
}

void clearScreen() {
    system("cls");
}

void printColoredText(const char* text, int color) {
    setColor(color);
    printf("%s", text);
    resetColor();
}

void mainMenuHeader() {
    setColor(COLOR_HEADER);
    printf("=== ");
    setColor(COLOR_TITLE);
    printf("NOTES APPLICATION");
    setColor(COLOR_HEADER);
    printf(" ===\n");
    resetColor();

    printf("-------------------------\n");

    setColor(COLOR_MENU);
    printf("1");
    resetColor();
    printf(" - Add a new note\n");

    setColor(COLOR_MENU);
    printf("2");
    resetColor();
    printf(" - View notes\n");

    setColor(COLOR_MENU);
    printf("3");
    resetColor();
    printf(" - Edit a note\n");

    setColor(COLOR_MENU);
    printf("4");
    resetColor();
    printf(" - Delete a note\n");

    setColor(COLOR_MENU);
    printf("0");
    resetColor();
    printf(" - Exit\n");

    printf("-------------------------\n");
}

void addNoteHeader() {
    setColor(COLOR_HEADER);
    printf("=== ");
    setColor(COLOR_TITLE);
    printf("ADD A NEW NOTE");
    setColor(COLOR_HEADER);
    printf(" ===\n");
    resetColor();
    printf("----------------------\n");
}

void viewNotesHeader() {
    setColor(COLOR_HEADER);
    printf("=== ");
    setColor(COLOR_TITLE);
    printf("VIEW NOTES");
    setColor(COLOR_HEADER);
    printf(" ===\n");
    resetColor();
    printf("------------------\n");
}

void editNoteHeader() {
    setColor(COLOR_HEADER);
    printf("=== ");
    setColor(COLOR_TITLE);
    printf("EDIT NOTE");
    setColor(COLOR_HEADER);
    printf(" ===\n");
    resetColor();
    printf("-----------------\n");
}

void deleteNoteHeader() {
    setColor(COLOR_HEADER);
    printf("=== ");
    setColor(COLOR_TITLE);
    printf("DELETE NOTE");
    setColor(COLOR_HEADER);
    printf(" ===\n");
    resetColor();
    printf("-------------------\n");
}

int getValidIntInput(int min, int max, const char* prompt) {
    int input;
    char buffer[256];
    int valid = 0;

    do {
        setColor(COLOR_PROMPT);
        printf("%s", prompt);
        resetColor();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        valid = 1;
        for (int i = 0; buffer[i] != '\n' && buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            input = atoi(buffer);
            if (input >= min && input <= max) {
                return input;
            }
            else {
                setColor(COLOR_ERROR);
                printf("Error: Input must be between %d and %d. Please try again.\n", min, max);
                resetColor();
                valid = 0;
            }
        }
        else {
            setColor(COLOR_ERROR);
            printf("Error: Please enter a valid number.\n");
            resetColor();
        }
    } while (!valid);

    return input;
}

char getYesNoInput(const char* prompt) {
    char choice;
    char buffer[10];
    int valid = 0;

    do {
        setColor(COLOR_PROMPT);
        printf("%s (y/n): ", prompt);
        resetColor();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        if (buffer[0] == 'y' || buffer[0] == 'Y' || buffer[0] == 'n' || buffer[0] == 'N') {
            choice = buffer[0];
            valid = 1;
        }
        else {
            setColor(COLOR_ERROR);
            printf("Error: Please enter 'y' for yes or 'n' for no.\n");
            resetColor();
        }
    } while (!valid);

    return choice;
}

void getNonEmptyStringInput(char* output, int maxLen, const char* prompt) {
    int valid = 0;

    do {
        setColor(COLOR_PROMPT);
        printf("%s", prompt);
        resetColor();

        if (fgets(output, maxLen, stdin) == NULL) {
            continue;
        }

        size_t len = strlen(output);
        if (len > 0 && output[len - 1] == '\n') {
            output[len - 1] = '\0';
            len--;
        }

        if (len > 0) {
            valid = 1;
        }
        else {
            setColor(COLOR_ERROR);
            printf("Error: Input cannot be empty. Please try again.\n");
            resetColor();
        }
    } while (!valid);
}

int directoryExists(const char* path) {
    DWORD attributes = GetFileAttributes(path);
    return (attributes != INVALID_FILE_ATTRIBUTES &&
        (attributes & FILE_ATTRIBUTE_DIRECTORY));
}

void createSampleNote(const char* title, const char* content) {
    char filename[MAX_PATH_LENGTH];
    char filePath[MAX_PATH_LENGTH];

    strcpy(filename, title);

    for (int i = 0; i < strlen(filename); i++) {
        if (filename[i] == ' ') {
            filename[i] = '_';
        }
    }

    sprintf(filePath, "%s\\%s.txt", NOTES_DIR, filename);

    FILE* file = fopen(filePath, "w");
    if (file != NULL) {
        fprintf(file, "%s", content);
        fclose(file);
        printf("Created sample note: ");
        setColor(COLOR_NOTE_TITLE);
        printf("%s\n", title);
        resetColor();
    }
    else {
        setColor(COLOR_ERROR);
        printf("Failed to create sample note: %s\n", title);
        resetColor();
    }
}

void initializeNotesDirectory() {
    if (!directoryExists(NOTES_DIR)) {
        printf("Notes directory not found. Creating directory with sample notes\n");

        if (_mkdir(NOTES_DIR) != 0) {
            setColor(COLOR_ERROR);
            printf("Failed to create notes directory. Please create it manually.\n");
            resetColor();
            return;
        }

        createSampleNote("NOTE 1", "I am NOTE 1 and I exist!");
        createSampleNote("NOTE 2", "I am NOTE 2 and I also exist!");
        createSampleNote("NOTE 3", "I am NOTE 3 and I also exist but better!");

        setColor(COLOR_SUCCESS);
        printf("Sample notes created successfully. ");
        resetColor();
        printf("Press Enter to continue");
        getchar();
    }
}

void loadNotesFromFiles() {
    noteCount = 0;

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPath[MAX_PATH_LENGTH];

    sprintf(searchPath, "%s\\*", NOTES_DIR);

    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        setColor(COLOR_ERROR);
        printf("Notes directory not found.\n");
        resetColor();
        return;
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char filepath[MAX_PATH_LENGTH];
            sprintf(filepath, "%s\\%s", NOTES_DIR, findFileData.cFileName);

            char title[MAX_TITLE_LENGTH];
            strncpy(title, findFileData.cFileName, MAX_TITLE_LENGTH - 1);
            title[MAX_TITLE_LENGTH - 1] = '\0';

            char* dot = strrchr(title, '.');
            if (dot != NULL) {
                *dot = '\0';
            }

            for (int i = 0; i < strlen(title); i++) {
                if (title[i] == '_') {
                    title[i] = ' ';
                }
            }

            FILE* file = fopen(filepath, "r");
            if (file != NULL && noteCount < MAX_NOTES) {
                strncpy(notes[noteCount].title, title, MAX_TITLE_LENGTH - 1);
                notes[noteCount].title[MAX_TITLE_LENGTH - 1] = '\0';
                size_t bytesRead = fread(notes[noteCount].content, 1, MAX_CONTENT_LENGTH - 1, file);
                notes[noteCount].content[bytesRead] = '\0';
                strncpy(notes[noteCount].filename, filepath, MAX_PATH_LENGTH - 1);
                notes[noteCount].filename[MAX_PATH_LENGTH - 1] = '\0';

                fclose(file);
                noteCount++;
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0 && noteCount < MAX_NOTES);

    FindClose(hFind);
}

void getMultiLineContent(char* content, int maxLength) {
    setColor(COLOR_PROMPT);
    printf("\nEnter note content (max %d characters):\n", maxLength - 1);
    printf("(Press Enter twice to finish)\n");
    resetColor();

    content[0] = '\0';
    char line[MAX_CONTENT_LENGTH];
    int contentLen = 0;

    while (1) {
        if (fgets(line, maxLength - contentLen, stdin) == NULL) {
            break;
        }

        if (line[0] == '\n') {
            break;
        }

        strcat(content, line);
        contentLen = strlen(content);

        if (contentLen >= maxLength - 3) {
            setColor(COLOR_ERROR);
            printf("Warning: Maximum content length reached.\n");
            resetColor();
            break;
        }
    }
}

void addNote() {
    clearScreen();
    addNoteHeader();

    char title[MAX_TITLE_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    char filename[MAX_PATH_LENGTH];
    char filepath[MAX_PATH_LENGTH];

    getNonEmptyStringInput(title, MAX_TITLE_LENGTH, "Enter note title (alphanumeric, max 49 chars): ");

    int existingNoteIndex = -1;
    for (int i = 0; i < noteCount; i++) {
        if (strcmp(notes[i].title, title) == 0) {
            existingNoteIndex = i;
            break;
        }
    }

    if (existingNoteIndex != -1) {
        printf("\nA note with the title \"");
        setColor(COLOR_NOTE_TITLE);
        printf("%s", title);
        resetColor();
        printf("\" already exists.\n");

        printf("Current content:\n");
        printf("%s\n", notes[existingNoteIndex].content);

        char choice = getYesNoInput("Do you want to edit this note?");

        if (choice != 'y' && choice != 'Y') {
            printf("Operation cancelled. Press enter to return to main menu...");
            getchar();
            clearScreen();
            return;
        }

        strcpy(filepath, notes[existingNoteIndex].filename);
    }
    else {
        strcpy(filename, title);
        for (int i = 0; i < strlen(filename); i++) {
            if (filename[i] == ' ') {
                filename[i] = '_';
            }
        }

        sprintf(filepath, "%s\\%s.txt", NOTES_DIR, filename);
    }

    getMultiLineContent(content, MAX_CONTENT_LENGTH);

    FILE* file = fopen(filepath, "w");
    if (file == NULL) {
        setColor(COLOR_ERROR);
        printf("Error: Unable to create/update note file.\n");
        resetColor();
        printf("Press enter to continue...");
        getchar();
        clearScreen();
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);

    if (existingNoteIndex != -1) {
        strcpy(notes[existingNoteIndex].content, content);
        setColor(COLOR_SUCCESS);
        printf("\nNote updated successfully!\n");
        resetColor();
    }
    else if (noteCount < MAX_NOTES) {
        strcpy(notes[noteCount].title, title);
        strcpy(notes[noteCount].content, content);
        strcpy(notes[noteCount].filename, filepath);
        noteCount++;
        setColor(COLOR_SUCCESS);
        printf("\nNote saved successfully!\n");
        resetColor();
    }
    else {
        setColor(COLOR_ERROR);
        printf("\nNote saved to file but not loaded in memory (maximum notes reached).\n");
        resetColor();
    }

    printf("Press enter to continue...");
    getchar();
    clearScreen();
}

void viewNotes() {
    clearScreen();
    viewNotesHeader();

    if (noteCount == 0) {
        setColor(COLOR_ERROR);
        printf("No notes found.\n");
        resetColor();
    }
    else {
        printf("Total notes: ");
        setColor(COLOR_MENU);
        printf("%d\n\n", noteCount);
        resetColor();

        for (int i = 0; i < noteCount; i++) {
            printf("Note #");
            setColor(COLOR_MENU);
            printf("%d\n", i + 1);
            resetColor();

            printf("Title: ");
            setColor(COLOR_NOTE_TITLE);
            printf("%s\n", notes[i].title);
            resetColor();

            printf("File: ");
            setColor(COLOR_FILENAME);
            printf("%s\n", notes[i].filename);
            resetColor();

            printf("Content:\n%s\n", notes[i].content);
            printf("------------------\n");
        }
    }

    printf("\nPress enter to continue...");
    getchar();
    clearScreen();
}

void editNote() {
    clearScreen();
    editNoteHeader();

    if (noteCount == 0) {
        setColor(COLOR_ERROR);
        printf("No notes found to edit.\n");
        resetColor();
        printf("Press enter to continue...");
        getchar();
        clearScreen();
        return;
    }

    printf("Available notes:\n");
    for (int i = 0; i < noteCount; i++) {
        setColor(COLOR_MENU);
        printf("%d", i + 1);
        resetColor();
        printf(" - ");
        setColor(COLOR_NOTE_TITLE);
        printf("%s\n", notes[i].title);
        resetColor();
    }

    char prompt[100];
    sprintf(prompt, "\nSelect note number to edit (1-%d): ", noteCount);
    int noteIndex = getValidIntInput(1, noteCount, prompt) - 1;

    printf("\nEditing: ");
    setColor(COLOR_NOTE_TITLE);
    printf("%s\n", notes[noteIndex].title);
    resetColor();

    printf("Current content:\n%s\n", notes[noteIndex].content);

    char content[MAX_CONTENT_LENGTH];
    getMultiLineContent(content, MAX_CONTENT_LENGTH);

    FILE* file = fopen(notes[noteIndex].filename, "w");
    if (file == NULL) {
        setColor(COLOR_ERROR);
        printf("Error: Unable to update note file.\n");
        resetColor();
        printf("Press enter to continue...");
        getchar();
        clearScreen();
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);

    strcpy(notes[noteIndex].content, content);
    setColor(COLOR_SUCCESS);
    printf("\nNote updated successfully!\n");
    resetColor();

    printf("Press enter to continue...");
    getchar();
    clearScreen();
}

void deleteNote() {
    clearScreen();
    deleteNoteHeader();

    if (noteCount == 0) {
        setColor(COLOR_ERROR);
        printf("No notes found to delete.\n");
        resetColor();
        printf("Press enter to continue...");
        getchar();
        clearScreen();
        return;
    }

    printf("Available notes:\n");
    for (int i = 0; i < noteCount; i++) {
        setColor(COLOR_MENU);
        printf("%d", i + 1);
        resetColor();
        printf(" - ");
        setColor(COLOR_NOTE_TITLE);
        printf("%s\n", notes[i].title);
        resetColor();
    }

    char prompt[100];
    sprintf(prompt, "\nSelect note number to delete (1-%d): ", noteCount);
    int noteIndex = getValidIntInput(1, noteCount, prompt) - 1;

    printf("\nYou are about to delete: ");
    setColor(COLOR_NOTE_TITLE);
    printf("%s\n", notes[noteIndex].title);
    resetColor();

    char choice = getYesNoInput("Are you sure you want to permanently delete this note?");

    if (choice != 'y' && choice != 'Y') {
        printf("Delete operation cancelled. Press enter to continue...");
        getchar();
        clearScreen();
        return;
    }

    if (remove(notes[noteIndex].filename) != 0) {
        setColor(COLOR_ERROR);
        printf("Error: Unable to delete the file.\n");
        resetColor();
        printf("Press enter to continue...");
        getchar();
        clearScreen();
        return;
    }

    for (int i = noteIndex; i < noteCount - 1; i++) {
        strcpy(notes[i].title, notes[i + 1].title);
        strcpy(notes[i].content, notes[i + 1].content);
        strcpy(notes[i].filename, notes[i + 1].filename);
    }

    noteCount--;

    setColor(COLOR_SUCCESS);
    printf("\nNote deleted successfully!\n");
    resetColor();

    printf("Press enter to continue...");
    getchar();
    clearScreen();
}

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
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    initializeNotesDirectory();
    loadNotesFromFiles();

    int option;
    do {
        mainMenuHeader();
        option = getValidIntInput(0, 4, "Enter option (0-4): ");
        menu(option);
    } while (option != 0);

    return 0;
}