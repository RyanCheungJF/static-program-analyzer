#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Tokenizer.h>
#include <Parser.h>
#include <DesignExtractor.h>

class SP {

public:
	Tokenizer tokenizer;
	Parser parser;
	DesignExtractor designExtractor;

	SP();

	void processFile(std::string filepath);

};