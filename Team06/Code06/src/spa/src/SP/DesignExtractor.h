#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "AST/Node/Program.h"

class DesignExtractor {

public:
	DesignExtractor() = default;

	void extractRelationships(Program* program);
};