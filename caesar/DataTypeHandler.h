#ifndef DATA_TYPE_HANDLER_H
#define DATA_TYPE_HANDLER_H

#include <string>
#include <vector>
#include "../main.h"

class DataTypeHandler {
private:
    Document* document;

public:
    DataTypeHandler(Document* doc);
    ~DataTypeHandler();

    // Line management
    void addTextLine(const std::string& text);
    void addContactLine(const std::string& name, const std::string& surname, const std::string& email);
    void addChecklistLine(const std::string& info, bool checked = false);

    // Line editing
    bool editTextLine(size_t lineIndex, const std::string& newText);
    bool editContactLine(size_t lineIndex, const std::string& name, const std::string& surname, const std::string& email);
    bool editChecklistLine(size_t lineIndex, const std::string& info, bool checked);

    // Line operations
    bool toggleChecklistItem(size_t lineIndex);
    bool deleteLine(size_t lineIndex);

    // Data conversion for encryption
    std::vector<char> serializeDocument();
    bool deserializeDocument(const std::vector<char>& data);

    // Display functions
    void printDocument();
    void printLine(size_t lineIndex);

    // Search functions
    std::vector<size_t> searchInDocument(const std::string& searchText);

    // Validation
    bool isValidLineIndex(size_t lineIndex) const;
    DataType getLineType(size_t lineIndex) const;

private:
    void ensureCapacity(size_t requiredCapacity);
    void freeLine(size_t lineIndex);
    std::string serializeLine(size_t lineIndex);
    bool deserializeLine(const std::string& data, size_t lineIndex);
};

#endif // DATA_TYPE_HANDLER_H