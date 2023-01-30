#pragma once
#include <iostream>

class SyntaxErrorException : public std::exception {
private:
	char* message;
public:
	SyntaxErrorException(char* msg) : message(msg) {}
	char* what() {
		return message;
	}
};

class SemanticErrorException : public std::exception {
private:
	char* message;
public:
	SemanticErrorException(char* msg) : message(msg) {}
	char* what() {
		return message;
	}
};