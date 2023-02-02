#include "StatementList.h"

void StatementList::accept(ASTVisitor *visitor) const{
	visitor->visitStatementList(this);
};