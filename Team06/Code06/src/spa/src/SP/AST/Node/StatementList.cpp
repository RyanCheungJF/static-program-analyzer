#include "StatementList.h"

StatementList::StatementList() {

}

void StatementList::accept(ASTVisitor *visitor) const{
	visitor->visitStatementList(this);
};