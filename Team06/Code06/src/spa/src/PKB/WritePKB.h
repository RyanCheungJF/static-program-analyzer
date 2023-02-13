#pragma once

#include "PKB.h"

typedef int StmtNum;

class WritePKB {
public:

    // Sets the pointer to the PKB instance if it is not set yet
    void setInstancePKB(PKB& pkb);

    // Sets Follows relation in PKB
    void setFollows(StmtNum left, StmtNum right);

    // Sets FollowsT relation in PKB
    void setFollowsT(StmtNum followee, std::unordered_set<StmtNum> followers);

    // Sets Parent relation in PKB
    void setParent(StmtNum parent, StmtNum child);

    // Sets ParentT relation in PKB
    void setParentT(StmtNum parent, std::unordered_set<StmtNum> children);

    // Sets the statement along with the statement lines that they appear in
    void setStatement(Stmt s, std::unordered_set<StmtNum> lines);

    // Sets the statement along with the statement line that it appears in
    void setStatement(Stmt s, StmtNum line);


    // Sets the entity along with the statement line that the entities appears in
    void setEntity(StmtNum line, std::unordered_set<Ent> entities);

    // Sets the entity along with the statement line that it appears in
    void setEntity(Ent e, StmtNum line);


    // Sets the procedure along with the statement lines that are in that procedure appears in
//    void setProcedure(Proc p, StmtNum num);

    // Sets the procedure along with the statement lines that are in that procedure appears in
    void setProcedure(Proc p, std::unordered_set<StmtNum> lines);

    // Sets the constants along with the statement line that the constants appears in
    void setConstant(StmtNum num, std::unordered_set<Const> constants);

    void setCall(StmtNum callLine, Proc procedure_being_called);

    void setUsesS(StmtNum num, std::unordered_set<Ent> entities);

    void setUsesP(ProcName, std::unordered_set<Ent> entities);

    void setModifiesS(StmtNum num, std::unordered_set<Ent> entities);

    void setModifiesP(ProcName, std::unordered_set<Ent> entities);

    void writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer);

    std::unique_ptr<Expression> buildSubtree(std::string rhs);

private:
    PKB* pkbInstance = NULL;
};