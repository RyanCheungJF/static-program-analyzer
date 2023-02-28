#pragma once

#include "DesignExtractor.h"
#include "Parser.h"
#include "SPExceptions.h"
#include "Tokenizer.h"

class SP {
private:
  Tokenizer tokenizer;
  Parser parser;
  DesignExtractor designExtractor;

public:
  void processFile(std::string filepath, WritePKB *writePKB, ReadPKB *readPKB);
};