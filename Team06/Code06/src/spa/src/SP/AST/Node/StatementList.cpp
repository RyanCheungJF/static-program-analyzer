#include "StatementList.h"

StatementList::StatementList(std::vector<Statement> statementList) {
	this->statementList = statementList;
};

void StatementList::accept(ASTVisitor visitor) {};