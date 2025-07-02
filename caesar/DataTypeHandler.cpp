#include "DataTypeHandler.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>

DataTypeHandler::DataTypeHandler(Document* doc) : document(doc) {
    if (!document) {
        std::cerr << "Error: Document pointer is null" << std::endl;
    }
}

DataTypeHandler::~DataTypeHandler() {
    // Note: We don't delete the document here as it's owned by the caller
}

void DataTypeHandler::ensureCapacity(size_t requiredCapacity) {
    if (document->capacity < requiredCapacity) {
        size_t newCapacity = std::max(requiredCapacity, document->capacity * 2);
        LineData* newLines = (LineData*)realloc(document->lines, newCapacity * sizeof(LineData));
        if (newLines) {
            document->lines = newLines;
            document->capacity = newCapacity;
        } else {
            std::cerr << "Error: Failed to allocate memory for lines" << std::endl;
        }
    }
}

void DataTypeHandler::addTextLine(const std::string& text) {
    ensureCapacity(document->lineCount + 1);

    LineData& line = document->lines[document->lineCount];
    line.type = DATA_TYPE_TEXT;
    line.data.text = (char*)malloc(text.length() + 1);
    if (line.data.text) {
        strcpy(line.data.text, text.c_str());
        document->lineCount++;
    } else {
        std::cerr << "Error: Failed to allocate memory for text line" << std::endl;
    }
}

void DataTypeHandler::addContactLine(const std::string& name, const std::string& surname, const std::string& email) {
    ensureCapacity(document->lineCount + 1);

    LineData& line = document->lines[document->lineCount];
    line.type = DATA_TYPE_CONTACT;

    // Copy data safely
    strncpy(line.data.contact.name, name.c_str(), sizeof(line.data.contact.name) - 1);
    line.data.contact.name[sizeof(line.data.contact.name) - 1] = '\0';

    strncpy(line.data.contact.surname, surname.c_str(), sizeof(line.data.contact.surname) - 1);
    line.data.contact.surname[sizeof(line.data.contact.surname) - 1] = '\0';

    strncpy(line.data.contact.email, email.c_str(), sizeof(line.data.contact.email) - 1);
    line.data.contact.email[sizeof(line.data.contact.email) - 1] = '\0';

    document->lineCount++;
}

void DataTypeHandler::addChecklistLine(const std::string& info, bool checked) {
    ensureCapacity(document->lineCount + 1);

    LineData& line = document->lines[document->lineCount];
    line.type = DATA_TYPE_CHECKLIST;

    strncpy(line.data.checklist.info, info.c_str(), sizeof(line.data.checklist.info) - 1);
    line.data.checklist.info[sizeof(line.data.checklist.info) - 1] = '\0';
    line.data.checklist.checked = checked ? 1 : 0;

    document->lineCount++;
}

bool DataTypeHandler::editTextLine(size_t lineIndex, const std::string& newText) {
    if (!isValidLineIndex(lineIndex) || document->lines[lineIndex].type != DATA_TYPE_TEXT) {
        return false;
    }

    free(document->lines[lineIndex].data.text);
    document->lines[lineIndex].data.text = (char*)malloc(newText.length() + 1);
    if (document->lines[lineIndex].data.text) {
        strcpy(document->lines[lineIndex].data.text, newText.c_str());
        return true;
    }
    return false;
}

bool DataTypeHandler::editContactLine(size_t lineIndex, const std::string& name, const std::string& surname, const std::string& email) {
    if (!isValidLineIndex(lineIndex) || document->lines[lineIndex].type != DATA_TYPE_CONTACT) {
        return false;
    }

    strncpy(document->lines[lineIndex].data.contact.name, name.c_str(), sizeof(document->lines[lineIndex].data.contact.name) - 1);
    document->lines[lineIndex].data.contact.name[sizeof(document->lines[lineIndex].data.contact.name) - 1] = '\0';

    strncpy(document->lines[lineIndex].data.contact.surname, surname.c_str(), sizeof(document->lines[lineIndex].data.contact.surname) - 1);
    document->lines[lineIndex].data.contact.surname[sizeof(document->lines[lineIndex].data.contact.surname) - 1] = '\0';

    strncpy(document->lines[lineIndex].data.contact.email, email.c_str(), sizeof(document->lines[lineIndex].data.contact.email) - 1);
    document->lines[lineIndex].data.contact.email[sizeof(document->lines[lineIndex].data.contact.email) - 1] = '\0';

    return true;
}

bool DataTypeHandler::editChecklistLine(size_t lineIndex, const std::string& info, bool checked) {
    if (!isValidLineIndex(lineIndex) || document->lines[lineIndex].type != DATA_TYPE_CHECKLIST) {
        return false;
    }

    strncpy(document->lines[lineIndex].data.checklist.info, info.c_str(), sizeof(document->lines[lineIndex].data.checklist.info) - 1);
    document->lines[lineIndex].data.checklist.info[sizeof(document->lines[lineIndex].data.checklist.info) - 1] = '\0';
    document->lines[lineIndex].data.checklist.checked = checked ? 1 : 0;

    return true;
}

bool DataTypeHandler::toggleChecklistItem(size_t lineIndex) {
    if (!isValidLineIndex(lineIndex) || document->lines[lineIndex].type != DATA_TYPE_CHECKLIST) {
        return false;
    }

    document->lines[lineIndex].data.checklist.checked = !document->lines[lineIndex].data.checklist.checked;
    return true;
}

bool DataTypeHandler::deleteLine(size_t lineIndex) {
    if (!isValidLineIndex(lineIndex)) {
        return false;
    }

    freeLine(lineIndex);

    // Move remaining lines down
    for (size_t i = lineIndex; i < document->lineCount - 1; i++) {
        document->lines[i] = document->lines[i + 1];
    }

    document->lineCount--;
    return true;
}

void DataTypeHandler::freeLine(size_t lineIndex) {
    if (isValidLineIndex(lineIndex) && document->lines[lineIndex].type == DATA_TYPE_TEXT) {
        free(document->lines[lineIndex].data.text);
        document->lines[lineIndex].data.text = nullptr;
    }
}

std::vector<char> DataTypeHandler::serializeDocument() {
    std::ostringstream oss;

    // Write header with line count
    oss << "DOCSTART:" << document->lineCount << "\n";

    for (size_t i = 0; i < document->lineCount; i++) {
        oss << serializeLine(i) << "\n";
    }

    oss << "DOCEND\n";

    std::string serialized = oss.str();
    return std::vector<char>(serialized.begin(), serialized.end());
}

std::string DataTypeHandler::serializeLine(size_t lineIndex) {
    if (!isValidLineIndex(lineIndex)) {
        return "";
    }

    std::ostringstream oss;
    const LineData& line = document->lines[lineIndex];

    switch (line.type) {
        case DATA_TYPE_TEXT:
            oss << "TEXT:" << (line.data.text ? line.data.text : "");
            break;
        case DATA_TYPE_CONTACT:
            oss << "CONTACT:" << line.data.contact.name << "|"
                << line.data.contact.surname << "|" << line.data.contact.email;
            break;
        case DATA_TYPE_CHECKLIST:
            oss << "CHECKLIST:" << (line.data.checklist.checked ? "1" : "0")
                << "|" << line.data.checklist.info;
            break;
    }

    return oss.str();
}

bool DataTypeHandler::deserializeDocument(const std::vector<char>& data) {
    std::string content(data.begin(), data.end());
    std::istringstream iss(content);
    std::string line;

    // Clear existing document
    for (size_t i = 0; i < document->lineCount; i++) {
        freeLine(i);
    }
    document->lineCount = 0;

    // Read header
    if (!std::getline(iss, line)) return false;
    if (line.substr(0, 9) != "DOCSTART:") return false;

    size_t expectedLines = std::stoul(line.substr(9));

    // Read lines
    size_t lineIndex = 0;
    while (std::getline(iss, line) && line != "DOCEND" && lineIndex < expectedLines) {
        if (!deserializeLine(line, lineIndex)) {
            return false;
        }
        lineIndex++;
    }

    return lineIndex == expectedLines;
}

bool DataTypeHandler::deserializeLine(const std::string& data, size_t lineIndex) {
    ensureCapacity(lineIndex + 1);

    if (data.substr(0, 5) == "TEXT:") {
        std::string text = data.substr(5);
        addTextLine(text);
        return true;
    } else if (data.substr(0, 8) == "CONTACT:") {
        std::string contactData = data.substr(8);
        size_t pos1 = contactData.find('|');
        size_t pos2 = contactData.find('|', pos1 + 1);
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            std::string name = contactData.substr(0, pos1);
            std::string surname = contactData.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string email = contactData.substr(pos2 + 1);
            addContactLine(name, surname, email);
            return true;
        }
    } else if (data.substr(0, 10) == "CHECKLIST:") {
        std::string checklistData = data.substr(10);
        size_t pos = checklistData.find('|');
        if (pos != std::string::npos) {
            bool checked = (checklistData.substr(0, pos) == "1");
            std::string info = checklistData.substr(pos + 1);
            addChecklistLine(info, checked);
            return true;
        }
    }

    return false;
}

void DataTypeHandler::printDocument() {
    std::cout << "\n=== Document Content ===" << std::endl;
    for (size_t i = 0; i < document->lineCount; i++) {
        std::cout << i << ": ";
        printLine(i);
    }
    std::cout << "=== End of Document ===" << std::endl;
}

void DataTypeHandler::printLine(size_t lineIndex) {
    if (!isValidLineIndex(lineIndex)) {
        std::cout << "[Invalid line]" << std::endl;
        return;
    }

    const LineData& line = document->lines[lineIndex];

    switch (line.type) {
        case DATA_TYPE_TEXT:
            std::cout << "[TEXT] " << (line.data.text ? line.data.text : "") << std::endl;
            break;
        case DATA_TYPE_CONTACT:
            std::cout << "[CONTACT] " << line.data.contact.name << " "
                      << line.data.contact.surname << " <" << line.data.contact.email << ">" << std::endl;
            break;
        case DATA_TYPE_CHECKLIST:
            std::cout << "[CHECKLIST] " << (line.data.checklist.checked ? "[✓]" : "[ ]")
                      << " " << line.data.checklist.info << std::endl;
            break;
    }
}

std::vector<size_t> DataTypeHandler::searchInDocument(const std::string& searchText) {
    std::vector<size_t> results;

    for (size_t i = 0; i < document->lineCount; i++) {
        const LineData& line = document->lines[i];
        bool found = false;

        switch (line.type) {
            case DATA_TYPE_TEXT:
                if (line.data.text && strstr(line.data.text, searchText.c_str())) {
                    found = true;
                }
                break;
            case DATA_TYPE_CONTACT:
                if (strstr(line.data.contact.name, searchText.c_str()) ||
                    strstr(line.data.contact.surname, searchText.c_str()) ||
                    strstr(line.data.contact.email, searchText.c_str())) {
                    found = true;
                }
                break;
            case DATA_TYPE_CHECKLIST:
                if (strstr(line.data.checklist.info, searchText.c_str())) {
                    found = true;
                }
                break;
        }

        if (found) {
            results.push_back(i);
        }
    }

    return results;
}

bool DataTypeHandler::isValidLineIndex(size_t lineIndex) const {
    return lineIndex < document->lineCount;
}

DataType DataTypeHandler::getLineType(size_t lineIndex) const {
    if (!isValidLineIndex(lineIndex)) {
        return DATA_TYPE_TEXT; // Default fallback
    }
    return document->lines[lineIndex].type;
}