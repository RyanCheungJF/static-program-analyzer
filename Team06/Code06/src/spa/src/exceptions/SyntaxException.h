#ifndef SPA_SYNTAXEXCEPTIONS_H
#define SPA_SYNTAXEXCEPTIONS_H
#include "Exception.h"
#include <string>

using namespace std;

class SyntaxException : public Exception {
private:
    const static string ERROR_MESSAGE;
public:
    SyntaxException();
};

#endif