#include "Program.h"

Program::Program() : procedureList() {}

Program::Program(std::vector<std::unique_ptr<Procedure>> procedureList) : procedureList(std::move(procedureList)) {}

void Program::accept(ASTVisitor* visitor) {
    visitor->visitProgram(this);
}