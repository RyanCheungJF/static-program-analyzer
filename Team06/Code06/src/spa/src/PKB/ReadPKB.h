#pragma once

#include "PKB.h"

class ReadPKB {
public:
    // Sets the pointer to the PKB instance if it is not set yet
    void setInstancePKB(PKB& pkb);

    std::vector<std::vector<std::string>> findRelationship(shared_ptr<Relationship> rs);

    std::vector<std::string> findDesignEntities(Parameter p);

    // check if given a statement type and statement line number, whether that
    // statement line number is indeed of that statement type
    bool checkStatement(Stmt stmt, StmtNum num);

    // returns all the statement lines that are contained in the given procedure
    std::unordered_set<StmtNum>& getProcedureStatementNumbers(ProcName p);

    // returns all the procedure names present in the source code
    std::unordered_set<ProcName>& getAllProcedureNames();

    // returns the entire row of all Entities involved in the Uses(StmtNum, v)
    // relationship
    std::unordered_set<Ent>& getUsesS(StmtNum num);

    // returns the entire row of all Entities involved in the Modifies(StmtNum, v)
    // relationship
    std::unordered_set<Ent>& getModifiesS(StmtNum num);

    // returns the entire row of all Entities involved in the Uses(ProcName, v)
    // relationship
    std::unordered_set<Ent>& getUsesP(ProcName name);

    // returns the entire row of all Entities involved in the Modifies(ProcName,
    // v) relationship
    std::unordered_set<Ent>& getModifiesP(ProcName name);

    // returns the name of the procedure being called on line number s
    // if line s is not a call statement, it returns a pair {AppConstants::NOT_USED_FIELD,
    // AppConstants::PROCEDURE_DOES_NOT_EXIST}
    std::pair<StmtNum, ProcName> getCallStmt(StmtNum s);

    // returns all the procedures that are called from a given procedure
    std::unordered_set<ProcName>& getCallsT(ProcName p);

    // returns all statement numbers for if statement
    std::unordered_set<StmtNum>& getIfStatementNumbers();

    // returns all statement numbers for while statement
    std::unordered_set<StmtNum>& getWhileStatementNumbers();

    // returns nested statement numbers of all if and while statements
    std::unordered_set<StmtNum>& getContainedStatements(StmtNum containerNum);

    // Returns relevant strings based on Pattern object passed
    std::vector<std::vector<std::string>> findPattern(Pattern p);

    // returns a pointer to the CFG graph
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>*
    getCFG(ProcName name); // todo: delete if unused

private:
    PKB* pkbInstance = NULL;
};
