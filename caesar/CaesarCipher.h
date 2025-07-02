#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
    typedef HMODULE LibraryHandle;
#else
#include <dlfcn.h>
typedef void* LibraryHandle;
#endif

class CaesarCipher {
private:
    LibraryHandle libraryHandle;

    // Function pointers for DLL functions
    typedef void (*EncryptFunction)(const char* input, char* output, int key, int length);
    typedef void (*DecryptFunction)(const char* input, char* output, int key, int length);

    EncryptFunction encryptFunc;
    DecryptFunction decryptFunc;

    bool loadLibrary();
    void unloadLibrary();
    bool loadFunctions();

public:
    CaesarCipher();
    ~CaesarCipher();

    // Main encryption/decryption methods
    std::vector<char> encrypt(const std::vector<char>& data, int key);
    std::vector<char> decrypt(const std::vector<char>& data, int key);

    // Convenience methods for strings
    std::string encrypt(const std::string& text, int key);
    std::string decrypt(const std::string& text, int key);

    // File encryption/decryption
    bool encryptFile(const std::string& inputPath, const std::string& outputPath, int key);
    bool decryptFile(const std::string& inputPath, const std::string& outputPath, int key);

    // Check if cipher is ready to use
    bool isReady() const;
};

#endif // CAESAR_CIPHER_H