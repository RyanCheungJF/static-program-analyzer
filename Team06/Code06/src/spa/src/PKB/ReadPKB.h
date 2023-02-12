#pragma once
#include "PKB.h"
#include "queryHandlers/StmtStmtRLHandler.h"
#include "queryHandlers/StmtEntRLHandler.h"
#include "queryHandlers/EntEntRLHandler.h"
#include "../qps/entities/Relationship.h"
#include "../qps/entities/Pattern.h"

//#ifndef SPA_READPKB_H
//#define SPA_READPKB_H
//#endif //SPA_READPKB_H

typedef int StmtNum;
typedef std::string ProcName;

class ReadPKB {
public:

    // Sets the pointer to the PKB instance if it is not set yet
    void setInstancePKB(PKB& pkb);

    // check if given a statement type and statement line number, whether that statement line number is indeed
    // of that statement type
    bool checkStatement(Stmt stmt, StmtNum num);

    // returns all the statement lines that are contained in the given procedure
    std::unordered_set<StmtNum> getProcedureStatementNumbers(Proc p);

    // returns all the call statement lines and the procedure that it is calling
    std::vector<std::pair<StmtNum, ProcName>> getCallStatements();

    // returns all the procedure names present in the source code
    std::unordered_set<ProcName> getAllProcedureNames();

    // returns the entire row of all Entities involved in the Uses(num, v) relationship
    std::unordered_set<Ent> getUsesS(StmtNum num);

    // returns all statement numbers for if statement
    std::unordered_set<StmtNum> getIfStatementNumbers();

    // returns all statement numbers for while statement
    std::unordered_set<StmtNum> getWhileStatementNumbers();


    // Returns relevant strings based on Pattern object passed
    std::vector<std::vector<std::string>> findPattern(Pattern p);



private:
    PKB* pkbInstance = NULL;
    std::unordered_map <RelationshipType, std::shared_ptr<StmtStmtRLStorage>> stmtStmtHandlerMap = { {RelationshipType::FOLLOWS, NULL} };
    std::unordered_map <RelationshipType, std::shared_ptr<StmtEntRLStorage>> stmtEntHandlerMap = {};
    std::unordered_map <RelationshipType, std::shared_ptr<EntEntRLStorage>> entEntHandlerMap = {};
        
};


