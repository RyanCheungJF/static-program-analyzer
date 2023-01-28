#include "Program.h"

Program::Program(std::vector<Procedure> procedureList, std::string fileName) {
	this->procedureList = procedureList;
	this->fileName = fileName;
};

void Program::accept(ASTVisitor visitor) {
	visitor->visitProgram(this);
};