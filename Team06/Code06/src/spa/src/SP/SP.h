#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <SPExceptions.h>
#include <Tokenizer.h>
#include <Parser.h>
#include <DesignExtractor.h>

class SP {
private:
	Tokenizer tokenizer;
	Parser parser;
	DesignExtractor designExtractor;

public:
	void processFile(std::string filepath);
};