#ifndef SPA_SEMANTICEXCEPTIONS_H
#define SPA_SEMANTICEXCEPTIONS_H
#include "Exception.h"
#include <string>
using namespace std;

class SemanticException : public Exception {
private:
    const static string ERROR_MESSAGE;
public:
    SemanticException();
};

#endif