#pragma once

#include "PKB.h"
#include "../utils/AppConstants.h"

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


    // Sets the statement along with the statement line that it appears in
    void setStatement(Stmt s, StmtNum line);


    // Sets the entity along with the statement line that the entities appears in
    void setEntity(StmtNum line, std::unordered_set<Ent> entities);

    // Sets the entity along with the statement line that it appears in
    void setEntity(Ent e, StmtNum line);

    // Sets the procedure along with the statement lines that are in that procedure appears in
    void setProcedure(ProcName p, std::unordered_set<StmtNum> lines);

    // Sets the constants along with the statement line that the constants appears in
    void setConstant(StmtNum num, std::unordered_set<Const> constants);

    void setCall(StmtNum callLine, ProcName procedure_being_called);

    void setUsesS(StmtNum num, std::unordered_set<Ent> entities);

    void setUsesP(ProcName, std::unordered_set<Ent> entities);

    void setModifiesS(StmtNum num, std::unordered_set<Ent> entities);

    void setModifiesP(ProcName, std::unordered_set<Ent> entities);


    void writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer);

    // Sets Uses relation in PKB
    void setUsesProcedure(std::string name, std::vector<Ent> entities);
    void setUsesStmtnum(StmtNum num, std::vector<Ent> entities);

    // Sets Modifies relation in PKB
    void setModifiesProcedure(std::string name, std::vector<Ent> entities);
    void setModifiesStmtnum(StmtNum num, std::vector<Ent> entities);

private:
    PKB* pkbInstance = NULL;
};