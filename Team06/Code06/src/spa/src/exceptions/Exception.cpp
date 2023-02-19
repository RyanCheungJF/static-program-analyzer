#include <utility>
#include "Exception.h"
using namespace std;

Exception::Exception() = default;

Exception::Exception(string message) {
    errorMessage = std::move(message);
}

string Exception::getMessage() {
    return errorMessage;
}