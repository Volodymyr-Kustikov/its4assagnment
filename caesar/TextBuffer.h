//#ifndef TEXTBUFFER_H
//#define TEXTBUFFER_H
//
//#include <vector>
//#include <string>
//
//class TextBuffer {
//private:
//    char* content;
//    size_t size;
//    size_t used;
//
//public:
//    TextBuffer(size_t initialSize = 1024);
//    ~TextBuffer();
//
//    // Copy constructor and assignment operator
//    TextBuffer(const TextBuffer& other);
//    TextBuffer& operator=(const TextBuffer& other);
//
//    // Getters
//    char* getContent() const { return content; }
//    size_t getSize() const { return size; }
//    size_t getUsed() const { return used; }
//
//    // Buffer operations
//    bool resize(size_t newSize);
//    bool setContent(const std::vector<char>& data);
//    std::vector<char> getContentAsVector() const;
//    bool isEmpty() const { return used == 0; }
//    void clear();
//
//    // C-style interface for compatibility
//    struct CTextBuffer {
//        char* content;
//        size_t size;
//        size_t used;
//    };
//
//    CTextBuffer* getCInterface();
//    void updateFromCInterface(const CTextBuffer* cBuffer);
//};
//
//#endif