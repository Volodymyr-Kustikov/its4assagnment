// caesar_dll.c - Simple Caesar cipher DLL implementation

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

// Simple Caesar cipher encryption function
EXPORT void caesar_encrypt(const char* input, char* output, int key, int length) {
    if (!input || !output || length <= 0) return;

    // Normalize key to 0-25 range
    key = ((key % 26) + 26) % 26;

    for (int i = 0; i < length; i++) {
        char c = input[i];

        if (c >= 'A' && c <= 'Z') {
            // Uppercase letters
            output[i] = ((c - 'A' + key) % 26) + 'A';
        } else if (c >= 'a' && c <= 'z') {
            // Lowercase letters
            output[i] = ((c - 'a' + key) % 26) + 'a';
        } else {
            // Non-alphabetic characters remain unchanged
            output[i] = c;
        }
    }
}

// Simple Caesar cipher decryption function
EXPORT void caesar_decrypt(const char* input, char* output, int key, int length) {
    if (!input || !output || length <= 0) return;

    // For decryption, we use the negative key
    caesar_encrypt(input, output, -key, length);
}

// Optional: DLL entry point for Windows
#ifdef _WIN32
#include <windows.h>

    BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
        switch (ul_reason_for_call) {
            case DLL_PROCESS_ATTACH:
            case DLL_THREAD_ATTACH:
            case DLL_THREAD_DETACH:
            case DLL_PROCESS_DETACH:
                break;
        }
        return TRUE;
    }
#endif