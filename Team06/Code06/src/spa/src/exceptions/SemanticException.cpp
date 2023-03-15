#include "SemanticException.h"

const string SemanticException::ERROR_MESSAGE = "SemanticError";

SemanticException::SemanticException() {
    this->errorMessage = ERROR_MESSAGE;
}