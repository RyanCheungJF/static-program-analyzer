#pragma once

#include <iostream>

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
    void processFile(const std::string& filepath, WritePKB* writePKB, ReadPKB* readPKB);
};