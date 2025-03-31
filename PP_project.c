#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h> 

#define MAX_NOTES 100
#define MAX_TITLE_LENGTH 50
#define MAX_CONTENT_LENGTH 2000
#define NOTES_DIR "notes"
#define MAX_PATH_LENGTH 260

// Structure to store a note
typedef struct {
    char title[MAX_TITLE_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    char filename[MAX_PATH_LENGTH];
} Note;

// Global array to store notes
Note notes[MAX_NOTES];
int noteCount = 0;

void clearScreen() {
    system("cls");
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

// Check if directory exists
int directoryExists(const char* path) {
    DWORD attributes = GetFileAttributes(path);
    return (attributes != INVALID_FILE_ATTRIBUTES &&
        (attributes & FILE_ATTRIBUTE_DIRECTORY));
}

// Create a sample note file
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
        printf("Created sample note: %s\n", title);
    }
    else {
        printf("Failed to create sample note: %s\n", title);
    }
}

void initializeNotesDirectory() {
    if (!directoryExists(NOTES_DIR)) {
        printf("Notes directory not found. Creating directory with sample notes\n");

        if (_mkdir(NOTES_DIR) != 0) {
            printf("Failed to create notes directory. Please create it manually.\n");
            return;
        }

        createSampleNote("NOTE 1", "I am NOTE 1 and I exist!");
        createSampleNote("NOTE 2", "I am NOTE 2 and I also exist!");
        createSampleNote("NOTE 3", "I am NOTE 3 and I also exist but better!");

        printf("Sample notes created successfully. Press Enter to continue");
        getchar();
    }
}

// Load notes from individual files in the notes directory
void loadNotesFromFiles() {
    noteCount = 0;

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPath[MAX_PATH_LENGTH];

    sprintf(searchPath, "%s\\*", NOTES_DIR);

    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Notes directory not found.\n");
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

    if (noteCount == 0) {
        printf("No notes found.\n");
    }
    else {
        printf("Total notes: %d\n\n", noteCount);

        for (int i = 0; i < noteCount; i++) {
            printf("Note #%d\n", i + 1);
            printf("Title: %s\n", notes[i].title);
            printf("File: %s\n", notes[i].filename);
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
    initializeNotesDirectory();
    loadNotesFromFiles();

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

    return 0;
}