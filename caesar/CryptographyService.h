#ifndef CRYPTOGRAPHYSERVICE_H
#define CRYPTOGRAPHYSERVICE_H

#include "CipherManager.h"
#include "main.h"
#include "InputHandler.h"
#include "FileManager.h"
#include "../main.h"

class CryptographyService {
private:
    CipherManager cipherManager;
    InputHandler inputHandler;
    FileManager fileManager;

public:
    CryptographyService();

    // Buffer operations
    void encryptCurrentText(TextBuffer& buffer);
    void decryptCurrentText(TextBuffer& buffer);

    // File operations
    void encryptTextFile();
    void decryptTextFile();

    // Save/Load operations
    void saveEncryptedText(TextBuffer& buffer);
    void loadEncryptedText(TextBuffer& buffer);

private:
    void saveBufferState(TextBuffer& buffer);
    bool updateBuffer(TextBuffer& buffer, const std::vector<char>& data);
};

#endif