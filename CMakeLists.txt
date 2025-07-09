cmake_minimum_required(VERSION 3.10)
project(TextEditor)

# Set C and C++ standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# Include subdirectories
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/caesar)
include_directories(${PROJECT_SOURCE_DIR}/cpp)

# Caesar dynamic library
add_library(caesar SHARED
        caesar/caesar_dll.c
        caesar/encryption_interface.cpp
        caesar/encryption_interface.h
        caesar/encryption_manager.cpp
        caesar/encryption_manager.h
)

# Main executable
add_executable(text_editor
        main.c
        cpp/additionalFunctionallity.cpp
        caesar/CaesarCipher.cpp
        caesar/DataTypeHandler.cpp
        caesar/TextEditorEncryption.cpp
)

# Link with the Caesar library
target_link_libraries(text_editor caesar dl)
