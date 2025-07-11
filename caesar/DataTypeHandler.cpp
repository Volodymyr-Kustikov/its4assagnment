#include "DataTypeHandler.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cstdlib>

DataTypeHandler::DataTypeHandler(Document* doc) : document(doc) {
    if (!document) {
        std::cerr << "Error: Document pointer is null" << std::endl;
    }
}

DataTypeHandler::~DataTypeHandler() {
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

void DataTypeHandler::addContactLine(const std::string& name, const std::string& surname, const std::string& email) {
    ensureCapacity(document->lineCount + 1);

    LineData& line = document->lines[document->lineCount];
    line.type = DATA_TYPE_CONTACT;

    strncpy(line.data.contact.name, name.c_str(), sizeof(line.data.contact.name) - 1);
    line.data.contact.name[sizeof(line.data.contact.name) - 1] = '\0';

    strncpy(line.data.contact.surname, surname.c_str(), sizeof(line.data.contact.surname) - 1);
    line.data.contact.surname[sizeof(line.data.contact.surname) - 1] = '\0';

    strncpy(line.data.contact.email, email.c_str(), sizeof(line.data.contact.email) - 1);
    line.data.contact.email[sizeof(line.data.contact.email) - 1] = '\0';

    document->lineCount++;
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
            std::cout << "[CHECKLIST] " << line.data.checklist.info
                      << " [" << (line.data.checklist.checked ? "X" : " ") << "]" << std::endl;
            break;
        default:
            std::cout << "[UNKNOWN TYPE]" << std::endl;
            break;
    }
}

bool DataTypeHandler::isValidLineIndex(size_t lineIndex) const {
    return lineIndex < document->lineCount;
}