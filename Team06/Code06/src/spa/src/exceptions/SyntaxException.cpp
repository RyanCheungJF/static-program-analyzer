#include "SyntaxException.h"

const string SyntaxException::ERROR_MESSAGE = "SyntaxError";

SyntaxException::SyntaxException()
{
	this->errorMessage = ERROR_MESSAGE;
}
