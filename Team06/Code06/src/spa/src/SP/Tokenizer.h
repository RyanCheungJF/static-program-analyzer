#pragma once

#include "../utils/AppConstants.h"
#include "SPExceptions.h"
#include "Token.h"
#include <deque>
#include <fstream>
#include <regex>
#include <sstream>

class Tokenizer {
public:
  bool isInteger(std::string value);
  std::deque<Token> tokenize(std::stringstream &file);
};