//#include "TextBuffer.h"
//#include <cstring>
//#include <cstdlib>
//
//TextBuffer::TextBuffer(size_t initialSize) : size(initialSize), used(0) {
//    content = (char*)malloc(size);
//    if (content) {
//        content[0] = '\0';
//    }
//}
//
//TextBuffer::~TextBuffer() {
//    if (content) {
//        free(content);
//    }
//}
//
//TextBuffer::TextBuffer(const TextBuffer& other) : size(other.size), used(other.used) {
//    content = (char*)malloc(size);
//    if (content && other.content) {
//        memcpy(content, other.content, used + 1);
//    }
//}
//
//TextBuffer& TextBuffer::operator=(const TextBuffer& other) {
//    if (this != &other) {
//        if (content) {
//            free(content);
//        }
//        size = other.size;
//        used = other.used;
//        content = (char*)malloc(size);
//        if (content && other.content) {
//            memcpy(content, other.content, used + 1);
//        }
//    }
//    return *this;
//}
//
//bool TextBuffer::resize(size_t newSize) {
//    if (newSize <= size) {
//        return true;
//    }
//
//    char* newBuffer = (char*)realloc(content, newSize);
//    if (!newBuffer) {
//        return false;
//    }
//
//    content = newBuffer;
//    size = newSize;
//    return true;
//}
//
//bool TextBuffer::setContent(const std::vector<char>& data) {
//    if (data.size() + 1 > size) {
//        if (!resize(data.size() + 1024)) {
//            return false;
//        }
//    }
//
//    memcpy(content, data.data(), data.size());
//    content[data.size()] = '\0';
//    used = data.size();
//    return true;
//}
//
//std::vector<char> TextBuffer::getContentAsVector() const {
//    return std::vector<char>(content, content + used);
//}
//
//void TextBuffer::clear() {
//    used = 0;
//    if (content) {
//        content[0] = '\0';
//    }
//}
//
//TextBuffer::CTextBuffer* TextBuffer::getCInterface() {
//    static CTextBuffer cBuffer;
//    cBuffer.content = content;
//    cBuffer.size = size;
//    cBuffer.used = used;
//    return &cBuffer;
//}
//
//void TextBuffer::updateFromCInterface(const CTextBuffer* cBuffer) {
//    if (cBuffer) {
//        content = cBuffer->content;
//        size = cBuffer->size;
//        used = cBuffer->used;
//    }
//}