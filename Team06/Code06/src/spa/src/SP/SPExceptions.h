#pragma once
#include <string>
#include <iostream>

class SyntaxErrorException : public std::exception {
private:
	std::string message;
public:
	SyntaxErrorException(const std::string msg) : message(msg) {}
	const char* what() {
		return (this->message).c_str();
	}
};

class SemanticErrorException : public std::exception {
private:
	std::string message;
public:
	SemanticErrorException(const std::string msg) : message(msg) {}
	const char* what() {
		return (this->message).c_str();
	}
};