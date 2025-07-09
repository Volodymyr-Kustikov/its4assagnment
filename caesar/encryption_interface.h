#include "encryption_manager.h"

// Global encryption manager instance
static EncryptionManager encryptionManager;

// C interface functions (extern "C" wrappers)
extern "C" void encryptCurrentText(TextBuffer* buffer) {
    encryptionManager.encryptCurrentText(buffer);
}

extern "C" void decryptCurrentText(TextBuffer* buffer) {
    encryptionManager.decryptCurrentText(buffer);
}

extern "C" void encryptTextFile() {
    encryptionManager.encryptTextFile();
}

extern "C" void decryptTextFile() {
    encryptionManager.decryptTextFile();
}

extern "C" void saveEncryptedText(TextBuffer* buffer) {
    encryptionManager.saveEncryptedText(buffer);
}

extern "C" void loadEncryptedText(TextBuffer* buffer) {
    encryptionManager.loadEncryptedText(buffer);
}