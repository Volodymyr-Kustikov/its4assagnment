#ifndef ENCRYPTION_MANAGER_H
#define ENCRYPTION_MANAGER_H

#include <string>
#include <vector>
#include "CaesarCipher.h"
#include "../main.h"

// Class for handling text buffer encryption operations
class TextBufferEncryption {
private:
    CaesarCipher* cipher;

public:
    TextBufferEncryption(CaesarCipher* cipherInstance);

    bool encryptBuffer(TextBuffer* buffer, int key);
    bool decryptBuffer(TextBuffer* buffer, int key);

private:
    bool resizeBuffer(TextBuffer* buffer, size_t newSize);
    void copyDataToBuffer(TextBuffer* buffer, const std::vector<char>& data);
};

// Class for handling file encryption operations
class FileEncryption {
private:
    CaesarCipher* cipher;

public:
    FileEncryption(CaesarCipher* cipherInstance);

    bool encryptFile(const std::string& inputPath, const std::string& outputPath, int key);
    bool decryptFile(const std::string& inputPath, const std::string& outputPath, int key);

    bool saveEncryptedBuffer(TextBuffer* buffer, const std::string& filename, int key);
    bool loadEncryptedBuffer(TextBuffer* buffer, const std::string& filename, int key);

private:
    std::vector<char> readFileToVector(const std::string& filename);
    bool writeVectorToFile(const std::vector<char>& data, const std::string& filename);
};

// Main class that coordinates encryption operations
class EncryptionManager {
private:
    CaesarCipher cipher;
    TextBufferEncryption textEncryption;
    FileEncryption fileEncryption;

public:
    EncryptionManager();

    bool isReady() const;

    // Text buffer operations
    void encryptCurrentText(TextBuffer* buffer);
    void decryptCurrentText(TextBuffer* buffer);

    // File operations
    void encryptTextFile();
    void decryptTextFile();
    void saveEncryptedText(TextBuffer* buffer);
    void loadEncryptedText(TextBuffer* buffer);

private:
    int getUserKey() const;
    std::string getUserFilename() const;
    void showError(const std::string& message) const;
    void showSuccess(const std::string& message) const;
};

#endif // ENCRYPTION_MANAGER_H