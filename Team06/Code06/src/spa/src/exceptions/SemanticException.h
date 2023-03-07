#ifndef SPA_SEMANTICEXCEPTIONS_H
#define SPA_SEMANTICEXCEPTIONS_H
#include <string>

#include "Exception.h"
using namespace std;

class SemanticException : public Exception {
private:
    const static string ERROR_MESSAGE;

public:
    SemanticException();
};

#endif