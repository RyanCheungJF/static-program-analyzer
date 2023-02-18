#ifndef SPA_INTERNALEXCEPTIONS_H
#define SPA_INTERNALEXCEPTIONS_H
#include "Exception.h"
#include <string>
using namespace std;

class InternalException : public Exception {
private:
    string errorMessage = "InternalException";
public:
    using Exception::Exception;
};

#endif