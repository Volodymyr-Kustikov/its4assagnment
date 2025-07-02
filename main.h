#ifndef MAIN_H
#define MAIN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Basic text buffer structure
typedef struct {
    char* content;
    size_t size;
    size_t used;
} TextBuffer;

// Data types for lines
typedef enum {
    DATA_TYPE_TEXT = 0,
    DATA_TYPE_CONTACT = 1,
    DATA_TYPE_CHECKLIST = 2
} DataType;

// Contact information structure
typedef struct {
    char name[100];
    char surname[100];
    char email[150];
} ContactInfo;

// Checklist item structure
typedef struct {
    char info[200];
    int checked; // 0 = unchecked, 1 = checked
} ChecklistItem;

// Line data structure
typedef struct {
    DataType type;
    union {
        char* text;
        ContactInfo contact;
        ChecklistItem checklist;
    } data;
} LineData;

// Document structure
typedef struct {
    LineData* lines;
    size_t lineCount;
    size_t capacity;
} Document;

// C function declarations
void displayMenu(void);
void processUserOption(int userOption);
void initializeDocument(void);
void freeDocument(void);

// Basic operations
void appendText(void);
void addNewLine(void);
void saveToFile(void);
void loadFromFile(void);
void printCurrentText(void);
void insertTextAtPosition(void);
void searchText(void);
void clearConsole(void);
void clearInputBuffer(void);

// Data type operations
void addContactLine(void);
void addChecklistLine(void);
void toggleChecklistItem(void);
void editContactLine(void);
void editChecklistLine(void);

// Encryption operations
void encryptDocument(void);
void decryptDocument(void);
void saveEncryptedDocument(void);
void loadEncryptedDocument(void);

#ifdef __cplusplus
}
#endif

#endif // MAIN_H