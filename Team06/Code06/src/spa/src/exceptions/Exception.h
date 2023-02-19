#ifndef SPA_EXCEPTIONS_H
#define SPA_EXCEPTIONS_H
#include <exception>
#include <string>
using namespace std;

class Exception : public exception {
protected:
    string errorMessage = "the programmer was too lazy to write a custom error message";
public:
    Exception();
    Exception(string);
    string getMessage();
};

#endif