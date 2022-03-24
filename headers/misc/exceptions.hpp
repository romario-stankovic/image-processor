#pragma once

#include <exception>

using namespace std;

struct FileNotFoundException : exception {
    const char* what() {
        return "File not found!";
    }
};

struct WrongFormatException : exception {
    const char* what() {
        return "Wrong file format!";
    }
};

struct BadFileFormatException : exception {
    const char* what() {
        return "Bad file format!";
    }
};

struct NotImplementedException : exception {
    const char* what() {
        return "Not implemented!";
    }
};

struct WriteException : exception {
    const char* what() {
        return "Writing failed";
    }
};