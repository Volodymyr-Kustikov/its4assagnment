#include "encryption_manager.h"
#include <iostream>
#include <fstream>
#include <cstring>

// ============= TextBufferEncryption Class =============

TextBufferEncryption::TextBufferEncryption(CaesarCipher* cipherInstance)
        : cipher(cipherInstance) {}

bool TextBufferEncryption::encryptBuffer(TextBuffer* buffer, int key) {
    if (buffer->used == 0) {
        return false;
    }

    // Save current state for undo
    saveState(buffer);

    // Convert buffer to vector
    std::vector<char> data(buffer->content, buffer->content + buffer->used);

    // Encrypt the data
    std::vector<char> encryptedData = cipher->encrypt(data, key);

    if (encryptedData.empty()) {
        return false;
    }

    // Update buffer with encrypted data
    if (!resizeBuffer(buffer, encryptedData.size() + 1)) {
        return false;
    }

    copyDataToBuffer(buffer, encryptedData);
    saveState(buffer);

    return true;
}

bool TextBufferEncryption::decryptBuffer(TextBuffer* buffer, int key) {
    if (buffer->used == 0) {
        return false;
    }

    // Save current state for undo
    saveState(buffer);

    // Convert buffer to vector
    std::vector<char> data(buffer->content, buffer->content + buffer->used);

    // Decrypt the data
    std::vector<char> decryptedData = cipher->decrypt(data, key);

    if (decryptedData.empty()) {
        return false;
    }

    // Update buffer with decrypted data
    if (!resizeBuffer(buffer, decryptedData.size() + 1)) {
        return false;
    }

    copyDataToBuffer(buffer, decryptedData);
    saveState(buffer);

    return true;
}

bool TextBufferEncryption::resizeBuffer(TextBuffer* buffer, size_t newSize) {
    if (newSize > buffer->size) {
        size_t actualNewSize = newSize + 1024;
        char* newBuffer = (char*)realloc(buffer->content, actualNewSize);
        if (!newBuffer) {
            return false;
        }
        buffer->content = newBuffer;
        buffer->size = actualNewSize;
    }
    return true;
}

void TextBufferEncryption::copyDataToBuffer(TextBuffer* buffer, const std::vector<char>& data) {
    memcpy(buffer->content, data.data(), data.size());
    buffer->content[data.size()] = '\0';
    buffer->used = data.size();
}

// ============= FileEncryption Class =============

FileEncryption::FileEncryption(CaesarCipher* cipherInstance)
        : cipher(cipherInstance) {}

bool FileEncryption::encryptFile(const std::string& inputPath, const std::string& outputPath, int key) {
    // Read input file
    std::vector<char> inputData = readFileToVector(inputPath);
    if (inputData.empty()) {
        return false;
    }

    // Encrypt the data
    std::vector<char> encryptedData = cipher->encrypt(inputData, key);
    if (encryptedData.empty()) {
        return false;
    }

    // Write encrypted data to output file
    return writeVectorToFile(encryptedData, outputPath);
}

bool FileEncryption::decryptFile(const std::string& inputPath, const std::string& outputPath, int key) {
    // Read encrypted file
    std::vector<char> encryptedData = readFileToVector(inputPath);
    if (encryptedData.empty()) {
        return false;
    }

    // Decrypt the data
    std::vector<char> decryptedData = cipher->decrypt(encryptedData, key);
    if (decryptedData.empty()) {
        return false;
    }

    // Write decrypted data to output file
    return writeVectorToFile(decryptedData, outputPath);
}

bool FileEncryption::saveEncryptedBuffer(TextBuffer* buffer, const std::string& filename, int key) {
    if (buffer->used == 0) {
        return false;
    }

    // Convert buffer to vector
    std::vector<char> data(buffer->content, buffer->content + buffer->used);

    // Encrypt the data
    std::vector<char> encryptedData = cipher->encrypt(data, key);
    if (encryptedData.empty()) {
        return false;
    }

    // Save to file
    return writeVectorToFile(encryptedData, filename);
}

bool FileEncryption::loadEncryptedBuffer(TextBuffer* buffer, const std::string& filename, int key) {
    // Save current state for undo
    saveState(buffer);

    // Read encrypted file
    std::vector<char> encryptedData = readFileToVector(filename);
    if (encryptedData.empty()) {
        return false;
    }

    // Decrypt the data
    std::vector<char> decryptedData = cipher->decrypt(encryptedData, key);
    if (decryptedData.empty()) {
        return false;
    }

    // Resize buffer if needed
    if (decryptedData.size() + 1 > buffer->size) {
        size_t newSize = decryptedData.size() + 1024;
        char* newBuffer = (char*)realloc(buffer->content, newSize);
        if (!newBuffer) {
            return false;
        }
        buffer->content = newBuffer;
        buffer->size = newSize;
    }

    // Copy decrypted data to buffer
    memcpy(buffer->content, decryptedData.data(), decryptedData.size());
    buffer->content[decryptedData.size()] = '\0';
    buffer->used = decryptedData.size();

    saveState(buffer);
    return true;
}

std::vector<char> FileEncryption::readFileToVector(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return std::vector<char>();
    }

    std::vector<char> data((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
    file.close();
    return data;
}

bool FileEncryption::writeVectorToFile(const std::vector<char>& data, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.write(data.data(), data.size());
    file.close();
    return true;
}

// ============= EncryptionManager Class =============

EncryptionManager::EncryptionManager()
        : textEncryption(&cipher), fileEncryption(&cipher) {}

bool EncryptionManager::isReady() const {
    return cipher.isReady();
}

void EncryptionManager::encryptCurrentText(TextBuffer* buffer) {
    if (!isReady()) {
        showError("Caesar cipher is not ready. Please check if the DLL is properly loaded.");
        return;
    }

    if (buffer->used == 0) {
        showError("Buffer is empty. Nothing to encrypt.");
        return;
    }

    int key = getUserKey();
    if (key == -1) return; // User input was invalid

    if (textEncryption.encryptBuffer(buffer, key)) {
        showSuccess("Text encrypted successfully with key " + std::to_string(key) + ".");
    } else {
        showError("Encryption failed.");
    }
}

void EncryptionManager::decryptCurrentText(TextBuffer* buffer) {
    if (!isReady()) {
        showError("Caesar cipher is not ready. Please check if the DLL is properly loaded.");
        return;
    }

    if (buffer->used == 0) {
        showError("Buffer is empty. Nothing to decrypt.");
        return;
    }

    int key = getUserKey();
    if (key == -1) return; // User input was invalid

    if (textEncryption.decryptBuffer(buffer, key)) {
        showSuccess("Text decrypted successfully with key " + std::to_string(key) + ".");
    } else {
        showError("Decryption failed.");
    }
}

void EncryptionManager::encryptTextFile() {
    if (!isReady()) {
        showError("Caesar cipher is not ready. Please check if the DLL is properly loaded.");
        return;
    }

    std::cout << "Enter input file path: ";
    std::string inputPath = getUserFilename();

    std::cout << "Enter output file path: ";
    std::string outputPath = getUserFilename();

    int key = getUserKey();
    if (key == -1) return;

    if (fileEncryption.encryptFile(inputPath, outputPath, key)) {
        showSuccess("File encryption completed successfully.");
    } else {
        showError("File encryption failed.");
    }
}

void EncryptionManager::decryptTextFile() {
    if (!isReady()) {
        showError("Caesar cipher is not ready. Please check if the DLL is properly loaded.");
        return;
    }

    std::cout << "Enter encrypted file path: ";
    std::string inputPath = getUserFilename();

    std::cout << "Enter output file path: ";
    std::string outputPath = getUserFilename();

    int key = getUserKey();
    if (key == -1) return;

    if (fileEncryption.decryptFile(inputPath, outputPath, key)) {
        showSuccess("File decryption completed successfully.");
    } else {
        showError("File decryption failed.");
    }
}

void EncryptionManager::saveEncryptedText(TextBuffer* buffer) {
    if (!isReady()) {
        showError("Caesar cipher is not ready. Please check if the DLL is properly loaded.");
        return;
    }

    if (buffer->used == 0) {
        showError("Buffer is empty. Nothing to save.");
        return;
    }

    std::cout << "Enter filename to save encrypted text: ";
    std::string filename = getUserFilename();

    int key = getUserKey();
    if (key == -1) return;

    if (fileEncryption.saveEncryptedBuffer(buffer, filename, key)) {
        showSuccess("Encrypted text saved successfully to: " + filename);
    } else {
        showError("Failed to save encrypted text.");
    }
}

void EncryptionManager::loadEncryptedText(TextBuffer* buffer) {
    if (!isReady()) {
        showError("Caesar cipher is not ready. Please check if the DLL is properly loaded.");
        return;
    }

    std::cout << "Enter filename to load encrypted text: ";
    std::string filename = getUserFilename();

    int key = getUserKey();
    if (key == -1) return;

    if (fileEncryption.loadEncryptedBuffer(buffer, filename, key)) {
        showSuccess("Encrypted text loaded and decrypted successfully from: " + filename);
    } else {
        showError("Failed to load encrypted text.");
    }
}

int EncryptionManager::getUserKey() const {
    int key;
    std::cout << "Enter encryption key (integer): ";
    if (!(std::cin >> key)) {
        showError("Invalid key format.");
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return -1;
    }
    std::cin.ignore(); // Clear the newline
    return key;
}

std::string EncryptionManager::getUserFilename() const {
    std::string filename;
    std::getline(std::cin, filename);
    return filename;
}

void EncryptionManager::showError(const std::string& message) const {
    std::cout << "Error: " << message << std::endl;
}

void EncryptionManager::showSuccess(const std::string& message) const {
    std::cout << message << std::endl;
}