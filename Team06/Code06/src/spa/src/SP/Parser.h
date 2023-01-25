#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

class Parser {

public:
	Parser();

	void parse(std::vector<std::string> tokens);

};