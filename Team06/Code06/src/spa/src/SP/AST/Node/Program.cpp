#include "Program.h"

Program::Program(std::string fileName) {
	this->fileName = fileName;
}

Program::Program() {}

void Program::accept(ASTVisitor* visitor) const {
	visitor->visitProgram(this);
}