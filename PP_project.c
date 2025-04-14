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

typedef struct {
    char title[MAX_TITLE_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    char filename[MAX_PATH_LENGTH];
} Note;

Note notes[MAX_NOTES];
int noteCount = 0;

void clearScreen() {
    system("cls");
}

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

void addNote() {
    clearScreen();
    addNoteHeader();

    char title[MAX_TITLE_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    char filename[MAX_PATH_LENGTH];
    char filepath[MAX_PATH_LENGTH];

    printf("Enter note title (max %d characters): ", MAX_TITLE_LENGTH - 1);
    fgets(title, MAX_TITLE_LENGTH, stdin);

    size_t len = strlen(title);
    if (len > 0 && title[len - 1] == '\n') {
        title[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("Title cannot be empty. Press enter to return to main menu...");
        getchar();
        clearScreen();
        return;
    }

    int existingNoteIndex = -1;
    for (int i = 0; i < noteCount; i++) {
        if (strcmp(notes[i].title, title) == 0) {
            existingNoteIndex = i;
            break;
        }
    }

    if (existingNoteIndex != -1) {
        printf("\nA note with the title \"%s\" already exists.\n", title);
        printf("Current content:\n%s\n", notes[existingNoteIndex].content);
        printf("\nDo you want to edit this note? (y/n): ");

        char choice;
        scanf("%c", &choice);
        getchar();

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

    printf("\nEnter note content (max %d characters):\n", MAX_CONTENT_LENGTH - 1);
    printf("(Press Enter twice to finish)\n");

    content[0] = '\0';
    char line[MAX_CONTENT_LENGTH];
    int contentLen = 0;

    while (1) {
        fgets(line, MAX_CONTENT_LENGTH - contentLen, stdin);

        if (line[0] == '\n') {
            break;
        }

        strcat(content, line);
        contentLen = strlen(content);

        if (contentLen >= MAX_CONTENT_LENGTH - 3) {
            break;
        }
    }

    FILE* file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Error: Unable to create/update note file.\n");
        printf("Press enter to continue...");
        getchar();
        clearScreen();
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);

    if (existingNoteIndex != -1) {
        strcpy(notes[existingNoteIndex].content, content);
        printf("\nNote updated successfully!\n");
    }
    else if (noteCount < MAX_NOTES) {
        strcpy(notes[noteCount].title, title);
        strcpy(notes[noteCount].content, content);
        strcpy(notes[noteCount].filename, filepath);
        noteCount++;
        printf("\nNote saved successfully!\n");
    }
    else {
        printf("\nNote saved to file but not loaded in memory (maximum notes reached).\n");
    }

    printf("Press enter to continue...");
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