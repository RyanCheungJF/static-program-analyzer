#ifndef SPA_SEMANTICEXCEPTIONS_H
#define SPA_SEMANTICEXCEPTIONS_H
#include "Exception.h"
#include <string>
using namespace std;

class SemanticException : public Exception {
private:
    string errorMessage = "SemanticError";
public:
    SemanticException();
};

#endif