#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "CaesarCipher.h"
#include "../main.h"

// Global cipher instance
static CaesarCipher cipher;

extern "C" void encryptCurrentText(TextBuffer* buffer) {
    if (!cipher.isReady()) {
        std::cout << "Caesar cipher is not ready. Please check if the DLL is properly loaded." << std::endl;
        return;
    }

    if (buffer->used == 0) {
        std::cout << "Buffer is empty. Nothing to encrypt." << std::endl;
        return;
    }

    int key;
    std::cout << "Enter encryption key (integer): ";
    if (!(std::cin >> key)) {
        std::cout << "Invalid key format." << std::endl;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return;
    }
    std::cin.ignore(); // Clear the newline

    // Save current state for undo
    saveState(buffer);

    // Convert buffer content to vector for encryption
    std::vector<char> data(buffer->content, buffer->content + buffer->used);

    // Encrypt the data
    std::vector<char> encryptedData = cipher.encrypt(data, key);

    if (encryptedData.empty()) {
        std::cout << "Encryption failed." << std::endl;
        return;
    }

    // Resize buffer if needed
    if (encryptedData.size() + 1 > buffer->size) {
        size_t newSize = encryptedData.size() + 1024;
        char* newBuffer = (char*)realloc(buffer->content, newSize);
        if (!newBuffer) {
            std::cout << "Memory allocation failed." << std::endl;
            return;
        }
        buffer->content = newBuffer;
        buffer->size = newSize;
    }

    // Copy encrypted data to buffer
    memcpy(buffer->content, encryptedData.data(), encryptedData.size());
    buffer->content[encryptedData.size()] = '\0';
    buffer->used = encryptedData.size();

    std::cout << "Text encrypted successfully with key " << key << "." << std::endl;
    saveState(buffer);
}

extern "C" void decryptCurrentText(TextBuffer* buffer) {
    if (!cipher.isReady()) {
        std::cout << "Caesar cipher is not ready. Please check if the DLL is properly loaded." << std::endl;
        return;
    }

    if (buffer->used == 0) {
        std::cout << "Buffer is empty. Nothing to decrypt." << std::endl;
        return;
    }

    int key;
    std::cout << "Enter decryption key (integer): ";
    if (!(std::cin >> key)) {
        std::cout << "Invalid key format." << std::endl;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return;
    }
    std::cin.ignore(); // Clear the newline

    // Save current state for undo
    saveState(buffer);

    // Convert buffer content to vector for decryption
    std::vector<char> data(buffer->content, buffer->content + buffer->used);

    // Decrypt the data
    std::vector<char> decryptedData = cipher.decrypt(data, key);

    if (decryptedData.empty()) {
        std::cout << "Decryption failed." << std::endl;
        return;
    }

    // Resize buffer if needed
    if (decryptedData.size() + 1 > buffer->size) {
        size_t newSize = decryptedData.size() + 1024;
        char* newBuffer = (char*)realloc(buffer->content, newSize);
        if (!newBuffer) {
            std::cout << "Memory allocation failed." << std::endl;
            return;
        }
        buffer->content = newBuffer;
        buffer->size = newSize;
    }

    // Copy decrypted data to buffer
    memcpy(buffer->content, decryptedData.data(), decryptedData.size());
    buffer->content[decryptedData.size()] = '\0';
    buffer->used = decryptedData.size();

    std::cout << "Text decrypted successfully with key " << key << "." << std::endl;
    saveState(buffer);
}

extern "C" void encryptTextFile() {
    if (!cipher.isReady()) {
        std::cout << "Caesar cipher is not ready. Please check if the DLL is properly loaded." << std::endl;
        return;
    }

    std::string inputPath, outputPath;
    int key;

    std::cout << "Enter input file path: ";
    std::getline(std::cin, inputPath);

    std::cout << "Enter output file path: ";
    std::getline(std::cin, outputPath);

    std::cout << "Enter encryption key (integer): ";
    if (!(std::cin >> key)) {
        std::cout << "Invalid key format." << std::endl;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return;
    }
    std::cin.ignore(); // Clear the newline

    if (cipher.encryptFile(inputPath, outputPath, key)) {
        std::cout << "File encryption completed successfully." << std::endl;
    } else {
        std::cout << "File encryption failed." << std::endl;
    }
}

extern "C" void decryptTextFile() {
    if (!cipher.isReady()) {
        std::cout << "Caesar cipher is not ready. Please check if the DLL is properly loaded." << std::endl;
        return;
    }

    std::string inputPath, outputPath;
    int key;

    std::cout << "Enter encrypted file path: ";
    std::getline(std::cin, inputPath);

    std::cout << "Enter output file path: ";
    std::getline(std::cin, outputPath);

    std::cout << "Enter decryption key (integer): ";
    if (!(std::cin >> key)) {
        std::cout << "Invalid key format." << std::endl;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return;
    }
    std::cin.ignore(); // Clear the newline

    if (cipher.decryptFile(inputPath, outputPath, key)) {
        std::cout << "File decryption completed successfully." << std::endl;
    } else {
        std::cout << "File decryption failed." << std::endl;
    }
}

extern "C" void saveEncryptedText(TextBuffer* buffer) {
    if (!cipher.isReady()) {
        std::cout << "Caesar cipher is not ready. Please check if the DLL is properly loaded." << std::endl;
        return;
    }

    if (buffer->used == 0) {
        std::cout << "Buffer is empty. Nothing to save." << std::endl;
        return;
    }

    std::string filename;
    int key;

    std::cout << "Enter filename to save encrypted text: ";
    std::getline(std::cin, filename);

    std::cout << "Enter encryption key (integer): ";
    if (!(std::cin >> key)) {
        std::cout << "Invalid key format." << std::endl;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return;
    }
    std::cin.ignore(); // Clear the newline

    // Convert buffer content to vector
    std::vector<char> data(buffer->content, buffer->content + buffer->used);

    // Encrypt the data
    std::vector<char> encryptedData = cipher.encrypt(data, key);

    if (encryptedData.empty()) {
        std::cout << "Encryption failed." << std::endl;
        return;
    }

    // Save encrypted data to file
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    file.write(encryptedData.data(), encryptedData.size());
    file.close();

    std::cout << "Encrypted text saved successfully to: " << filename << std::endl;
}

extern "C" void loadEncryptedText(TextBuffer* buffer) {
    if (!cipher.isReady()) {
        std::cout << "Caesar cipher is not ready. Please check if the DLL is properly loaded." << std::endl;
        return;
    }

    std::string filename;
    int key;

    std::cout << "Enter filename to load encrypted text: ";
    std::getline(std::cin, filename);

    std::cout << "Enter decryption key (integer): ";
    if (!(std::cin >> key)) {
        std::cout << "Invalid key format." << std::endl;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return;
    }
    std::cin.ignore(); // Clear the newline

    // Save current state for undo
    saveState(buffer);

    // Read encrypted file
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open file for reading: " << filename << std::endl;
        return;
    }

    // Read file content into vector
    std::vector<char> encryptedData((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
    file.close();

    if (encryptedData.empty()) {
        std::cout << "File is empty or could not be read." << std::endl;
        return;
    }

    // Decrypt the data
    std::vector<char> decryptedData = cipher.decrypt(encryptedData, key);

    if (decryptedData.empty()) {
        std::cout << "Decryption failed." << std::endl;
        return;
    }

    // Resize buffer if needed
    if (decryptedData.size() + 1 > buffer->size) {
        size_t newSize = decryptedData.size() + 1024;
        char* newBuffer = (char*)realloc(buffer->content, newSize);
        if (!newBuffer) {
            std::cout << "Memory allocation failed." << std::endl;
            return;
        }
        buffer->content = newBuffer;
        buffer->size = newSize;
    }

    // Copy decrypted data to buffer
    memcpy(buffer->content, decryptedData.data(), decryptedData.size());
    buffer->content[decryptedData.size()] = '\0';
    buffer->used = decryptedData.size();

    std::cout << "Encrypted text loaded and decrypted successfully from: " << filename << std::endl;
    saveState(buffer);
}