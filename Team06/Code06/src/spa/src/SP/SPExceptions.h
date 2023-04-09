#pragma once

#include <iostream>
#include <string>

class SyntaxErrorException : public std::exception {
private:
    std::string message;

public:
    explicit SyntaxErrorException(const std::string msg) : message(msg) {}
    const char* what() {
        return (this->message).c_str();
    }
};

class SemanticErrorException : public std::exception {
private:
    std::string message;

public:
    explicit SemanticErrorException(const std::string msg) : message(msg) {}
    const char* what() {
        return (this->message).c_str();
    }
};

class FileNotFoundException : public std::exception {
private:
    std::string message;

public:
    explicit FileNotFoundException(const std::string msg) : message(msg) {}
    const char* what() {
        return (this->message).c_str();
    }
};