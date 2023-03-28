#pragma once

#include "PKB.h"

class WritePKB {
public:
    // Sets the pointer to the PKB instance if it is not set yet
    void setInstancePKB(PKB& pkb);

    // Sets Follows relation in PKB
    void setFollows(StmtNum left, StmtNum right);

    // Sets FollowsT relation in PKB
    void setFollowsT(StmtNum followee, std::unordered_set<StmtNum>& followers);

    // Sets Parent relation in PKB
    void setParent(StmtNum parent, StmtNum child);

    // Sets ParentT relation in PKB
    void setParentT(StmtNum parent, std::unordered_set<StmtNum>& children);

    // Sets the statement along with the statement line that it appears in
    void setStatement(Stmt s, StmtNum line);

    // Sets the entity along with the statement line that the entities appears in
    void setEntity(StmtNum line, std::unordered_set<Ent>& entities);

    // Sets the entity along with the statement line that it appears in
    void setEntity(Ent e, StmtNum line);

    // Sets the procedure along with the statement lines that are in that
    // procedure appears in
    void setProcedure(ProcName p, std::unordered_set<StmtNum>& lines);

    // Sets the procedure along with the assign statement lines that are in that procedure
    void setProcAssignStmt(ProcName p, StmtNum num);

    // Sets the constants along with the statement line that the constants appears
    // in
    void setConstant(StmtNum num, std::unordered_set<Const>& constants);

    void setCall(StmtNum callLine, ProcName procedure_being_called);

    void setCalls(ProcName caller, std::unordered_set<ProcName>& callees);

    void setCallsT(ProcName caller, std::unordered_set<ProcName>& callees);

    void setUsesS(StmtNum num, std::unordered_set<Ent>& entities);

    void setUsesP(ProcName, std::unordered_set<Ent>& entities);

    void setModifiesS(StmtNum num, std::unordered_set<Ent>& entities);

    void setModifiesP(ProcName, std::unordered_set<Ent>& entities);

    void setIfPattern(StmtNum num, std::unordered_set<Ent>& variables);

    void setWhilePattern(StmtNum num, std::unordered_set<Ent>& variables);

    void writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer);

    void writeCFG(ProcName name,
                  std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& graph);

private:
    PKB* pkbInstance = NULL;
};