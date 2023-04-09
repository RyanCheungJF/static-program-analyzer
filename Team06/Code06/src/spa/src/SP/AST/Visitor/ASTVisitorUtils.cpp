#include "ASTVisitorUtils.h"

StmtNum visitLastStatementHelper(Statement* statement) {
    if (const auto ifStmt = CAST_TO(IfStatement, statement)) {
        auto statementList = ifStmt->elseStmtList.get();
        return checkLastStatementHelper(statementList);
    }
    else { // Else it must be a while statement
        const auto whileStmt = CAST_TO(WhileStatement, statement);
        auto statementList = whileStmt ? whileStmt->stmtList.get() : nullptr;
        return checkLastStatementHelper(statementList);
    }
}

StmtNum checkLastStatementHelper(StatementList* stmtList) {
    if (CAST_TO(IfStatement, stmtList->getLastStatement())) {
        return visitLastStatementHelper(stmtList->getLastStatement());
    }
    else if (CAST_TO(WhileStatement, stmtList->getLastStatement())) {
        return visitLastStatementHelper(stmtList->getLastStatement());
    }
    else {
        return stmtList->getLastStatementNumber();
    }
}

void visitCondExprHelper(ConditionalExpression* condExpr, std::unordered_set<Ent>& variables,
                         std::unordered_set<Const>& constants) {
    if (const auto notCondExpr = CAST_TO(NotConditionalExpression, condExpr)) {
        visitCondExprHelper(notCondExpr->condExpr.get(), variables, constants);
    }
    else if (const auto binCondExpr = CAST_TO(BinaryConditionalExpression, condExpr)) {
        visitCondExprHelper(binCondExpr->lhs.get(), variables, constants);
        visitCondExprHelper(binCondExpr->rhs.get(), variables, constants);
    }
    else if (const auto relExpr = CAST_TO(RelationalExpression, condExpr)) {
        visitExprHelper(relExpr->lhs.get(), variables, constants);
        visitExprHelper(relExpr->rhs.get(), variables, constants);
    }
}

void visitExprHelper(Expression* expr, std::unordered_set<Ent>& variables, std::unordered_set<Const>& constants) {
    if (const auto mathExpr = CAST_TO(MathExpression, expr)) {
        visitExprHelper(mathExpr->lhs.get(), variables, constants);
        visitExprHelper(mathExpr->rhs.get(), variables, constants);
    }
    else if (const auto constant = CAST_TO(Constant, expr)) {
        constants.insert(constant->value);
    }
    else if (const auto variable = CAST_TO(Variable, expr)) {
        variables.insert(variable->name);
    }
}

void recurseStatementHelper(Statement* recurseStmt, ASTVisitor* visitor) {
    if (const auto ifStmt = CAST_TO(IfStatement, recurseStmt)) {
        ifStmt->thenStmtList->accept(visitor);
        checkStatementHelper(ifStmt->getThenStatements(), visitor);

        ifStmt->elseStmtList->accept(visitor);
        checkStatementHelper(ifStmt->getElseStatements(), visitor);
    }
    else if (const auto whileStmt = CAST_TO(WhileStatement, recurseStmt)) {
        whileStmt->stmtList->accept(visitor);
        checkStatementHelper(whileStmt->getStatements(), visitor);
    }
}

void checkStatementHelper(std::vector<std::unique_ptr<Statement>>& statements, ASTVisitor* visitor) {
    for (const auto& statement : statements) {
        statement->accept(visitor);
        if (isContainerStatement(statement.get())) {
            recurseStatementHelper(statement.get(), visitor);
        }
    }
}

bool isContainerStatement(Statement* statement) {
    return CAST_TO(IfStatement, statement) || CAST_TO(WhileStatement, statement);
}