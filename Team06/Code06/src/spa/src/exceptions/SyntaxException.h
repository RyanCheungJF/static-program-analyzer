#ifndef SPA_SYNTAXEXCEPTIONS_H
#define SPA_SYNTAXEXCEPTIONS_H
#include <string>

#include "Exception.h"

using namespace std;

class SyntaxException : public Exception {
private:
    const static string ERROR_MESSAGE;

public:
    SyntaxException();
};

#endif