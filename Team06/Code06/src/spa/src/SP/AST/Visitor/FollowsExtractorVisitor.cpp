#include "FollowsExtractorVisitor.h"

FollowsExtractorVisitor::FollowsExtractorVisitor() {};

void FollowsExtractorVisitor::visitProgram(std::shared_ptr<Program> program) {};

void FollowsExtractorVisitor::visitProcedure(std::shared_ptr<Procedure> procedure) {};

void FollowsExtractorVisitor::visitReadStatement(std::shared_ptr<ReadStatement> readStatement) {};

void FollowsExtractorVisitor::visitPrintStatement(std::shared_ptr<PrintStatement> printStatement) {};

void FollowsExtractorVisitor::visitAssignStatement(std::shared_ptr<AssignStatement> assignStatement) {};

void FollowsExtractorVisitor::visitCallStatement(std::shared_ptr<CallStatement> callStatement) {};

void FollowsExtractorVisitor::visitIfElseStatement(std::shared_ptr<IfElseStatement> IfElseStatement) {};

void FollowsExtractorVisitor::visitWhileStatement(std::shared_ptr<WhileStatement> whileStatement) {};