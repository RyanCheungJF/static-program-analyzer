#ifndef SPA_INTERNALEXCEPTIONS_H
#define SPA_INTERNALEXCEPTIONS_H
#include <string>

#include "Exception.h"
using namespace std;

class InternalException : public Exception {
private:
    const static string ERROR_MESSAGE;

public:
    using Exception::Exception;
};

#endif