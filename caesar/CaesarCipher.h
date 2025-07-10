#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

#include <vector>
#include <string>

// For Mac OS dynamic library loading
#include <dlfcn.h>
typedef void* LibraryHandle;

class CaesarCipher {
private:
    LibraryHandle libraryHandle;

    // Function pointers for library functions
    typedef void (*EncryptFunction)(const char* input, char* output, int key, int length);
    typedef void (*DecryptFunction)(const char* input, char* output, int key, int length);

    EncryptFunction encryptFunc;
    DecryptFunction decryptFunc;

    // Helper methods
    bool loadLibrary();
    void unloadLibrary();
    bool loadFunctions();

public:
    CaesarCipher();
    ~CaesarCipher();

    // Main encryption/decryption methods
    std::vector<char> encrypt(const std::vector<char>& data, int key);
    std::vector<char> decrypt(const std::vector<char>& data, int key);

    // String convenience methods
    std::string encrypt(const std::string& text, int key);
    std::string decrypt(const std::string& text, int key);

    // File operations
    bool encryptFile(const std::string& inputPath, const std::string& outputPath, int key);
    bool decryptFile(const std::string& inputPath, const std::string& outputPath, int key);

    // Status check
    bool isReady() const;
};

#endif // CAESAR_CIPHER_H