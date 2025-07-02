#ifndef MAIN_H
#define MAIN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char* content;
    size_t size;
    size_t used;
} TextBuffer;

typedef struct {
    char* states[10];
    size_t stateSizes[10];
    int currentIndex;
    int totalStates;
} UndoRedoHistory;

extern UndoRedoHistory history;

// C function declarations
void displayMenu(void);
void processuserOption(int userOption, TextBuffer* buffer);
void initializeBuffer(TextBuffer* buffer);
void resizeBufferIfNeeded(TextBuffer* buffer, size_t additionalSpace);
void freeBuffer(TextBuffer* buffer);

void appendText(TextBuffer* buffer);
void addNewLine(TextBuffer* buffer);
void saveToFile(TextBuffer* buffer);
void loadFromFile(TextBuffer* buffer);
void printCurrentText(TextBuffer* buffer);
void insertTextAtPosition(TextBuffer* buffer);
void searchText(TextBuffer* buffer);

void clearConsole(void);
void clearInputBuffer(void);

// C++ function declarations (implemented in additionalFunctionallity.cpp)
void deleteText(TextBuffer* buffer);
void initHistory(void);
void saveState(TextBuffer* buffer);
void undoCommand(TextBuffer* buffer);
void redoCommand(TextBuffer* buffer);
void freeHistory(void);

// New function declarations for cut/copy/paste and insert with replacement
void copyText(TextBuffer* buffer);
void cutText(TextBuffer* buffer);
void pasteText(TextBuffer* buffer);
void insertWithReplacement(TextBuffer* buffer);

void encryptCurrentText(TextBuffer* buffer);
void decryptCurrentText(TextBuffer* buffer);
void encryptTextFile(void);
void decryptTextFile(void);
void saveEncryptedText(TextBuffer* buffer);
void loadEncryptedText(TextBuffer* buffer);

#ifdef __cplusplus
}
#endif

#endif