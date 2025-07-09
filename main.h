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

// Undo/Redo history structure
typedef struct {
    char* states[10];        // Array to store 10 different states
    size_t stateSizes[10];   // Size of each saved state
    int currentIndex;        // Current position in the history
    int totalStates;         // Total number of states saved
} UndoRedoHistory;

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

// Core operations
void displayMenu(void);
void processUserOption(int userOption, TextBuffer* buffer);
void initializeBuffer(TextBuffer* buffer);
void resizeBufferIfNeeded(TextBuffer* buffer, size_t additionalSpace);
void freeBuffer(TextBuffer* buffer);
void clearConsole(void);
void clearInputBuffer(void);

// Text buffer actions
void appendText(TextBuffer* buffer);
void addNewLine(TextBuffer* buffer);
void saveToFile(TextBuffer* buffer);
void loadFromFile(TextBuffer* buffer);
void printCurrentText(TextBuffer* buffer);
void insertTextAtPosition(TextBuffer* buffer);
void searchText(TextBuffer* buffer);
void deleteText(TextBuffer* buffer);

// Undo/redo clipboard
void initHistory(void);
void saveState(TextBuffer* buffer);
void undoCommand(TextBuffer* buffer);
void redoCommand(TextBuffer* buffer);
void pasteText(TextBuffer* buffer);
void copyText(TextBuffer* buffer);
void cutText(TextBuffer* buffer);
void insertWithReplacement(TextBuffer* buffer);
void freeHistory(void);

// Encryption
void encryptCurrentText(TextBuffer* buffer);
void decryptCurrentText(TextBuffer* buffer);
void encryptTextFile(void);
void decryptTextFile(void);
void saveEncryptedText(TextBuffer* buffer);
void loadEncryptedText(TextBuffer* buffer);

#ifdef __cplusplus
}
#endif

#endif // MAIN_H