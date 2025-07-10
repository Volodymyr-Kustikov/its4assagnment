#ifndef CIPHERMANAGER_H
#define CIPHERMANAGER_H

#include "CaesarCipher.h"
#include "main.h"
#include <vector>
#include <string>

class CipherManager {
private:
    CaesarCipher cipher;

public:
    CipherManager();

    bool isReady() const { return cipher.isReady(); }

    // Text operations
    std::vector<char> encryptText(const std::vector<char>& data, int key);
    std::vector<char> decryptText(const std::vector<char>& data, int key);

    // File operations
    bool encryptFile(const std::string& inputPath, const std::string& outputPath, int key);
    bool decryptFile(const std::string& inputPath, const std::string& outputPath, int key);

    // Validation
    bool validateKey(int key) const;
    bool validateData(const std::vector<char>& data) const;
};

#endif