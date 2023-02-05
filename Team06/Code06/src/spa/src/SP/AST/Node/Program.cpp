#include "Program.h"

Program::Program() {}

Program::Program(std::vector<std::unique_ptr<Procedure>> procedureList) {
	this->procedureList = std::move(procedureList);
}

void Program::accept(ASTVisitor* visitor) const {
	visitor->visitProgram(this);
}