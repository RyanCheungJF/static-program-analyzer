#include "Program.h"

void Program::accept(ASTVisitor* visitor) const {
	visitor->visitProgram(this);
}