#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include <deque>

typedef std::string Ent;
typedef std::string StmtType;
typedef std::string ProcedureName;
typedef int StmtNum;

class UsesStorage {
public:
    virtual void writeUses(ProcedureName name, StmtType type, StmtNum num, Ent ent);

    virtual void writeUsesCall(ProcedureName caller, ProcedureName callee, StmtNum num);

    virtual std::unordered_set<std::string> getAllEntitiesUsed(ProcedureName name);

    virtual bool checkUses(StmtNum num, Ent e); // Ent is either "x" or _

    virtual bool checkUses(ProcedureName name, Ent e);

    //TODO: given a line, check what statement is it.
    // For queries like: Select v such that Uses(1, v)

    // Select pn such that Uses(pn, v) or Select v such that Uses(pn, v)
    virtual std::vector<std::vector<std::string>> getUsesAllPrintStatements();

    // Select pn such that Uses("proc1", v) or Select pn such that Uses("proc1", _)
    virtual std::vector<std::vector<std::string>> getUsesAllPrintStatementsGivenProcedure(ProcedureName name);

    // Select pn such that Uses(pn, "x")
    virtual std::vector<std::vector<std::string>> getUsesAllPrintStatementsGivenEntity(Ent entity);

    // Select a such that Uses(a, v)
    virtual std::vector<std::vector<std::string>> getUsesAllAssignStatements();

    // Select a such that Uses("proc1", v) or Select pn such that Uses("proc1", _)
    virtual std::vector<std::vector<std::string>> getUsesAllAssignStatementsGivenProcedure(ProcedureName name);

    // Select a such that Uses(a, "x")
    virtual std::vector<std::vector<std::string>> getUsesAllAssignStatementsGivenEntity(Ent entity);

    // Select if such that Uses(if, v)
    virtual std::vector<std::vector<std::string>> getUsesAllIfStatements();

    // Select if such that Uses("proc1", v) or Select pn such that Uses("proc1", _)
    virtual std::vector<std::vector<std::string>> getUsesAllIfStatementsGivenProcedure(ProcedureName name);

    // Select if such that Uses(if, "x")
    virtual std::vector<std::vector<std::string>> getUsesAllIfStatementsGivenEntity(Ent entity);

    // Select w such that Uses(w, v)
    virtual std::vector<std::vector<std::string>> getUsesAllWhileStatements();

    // Select w such that Uses("proc1", v) or Select pn such that Uses("proc1", _)
    virtual std::vector<std::vector<std::string>> getUsesAllWhileStatementsGivenProcedure(ProcedureName name);

    // Select w such that Uses(w, "x")
    virtual std::vector<std::vector<std::string>> getUsesAllWhileStatementsGivenEntity(Ent entity);

    /*
    // Select c such that Uses(c, v)
    virtual std::vector<std::vector<std::string>> getUsesCallAll();

    // Select c such that Uses(c, "x")
    virtual std::vector<std::vector<std::string>> getUsesCallGivenVariable(std::string rhs);
    //TODO: need to pre-process nested procedure calls at compile time

    // Select s such that Uses(s, v)
    virtual std::vector<std::vector<std::string>> getUsesStatementAll();

    // Select s such that Uses(s, "x")
    virtual std::vector<std::vector<std::string>> getUsesStatementAllGivenVariable(std::string rhs);

    // Select p such that Uses(p, v)
    virtual std::vector<std::vector<std::string>> getUsesProcedureAll();

    // Select p such that Uses(p, "x")
    virtual std::vector<std::vector<std::string>> getUsesProcedureGivenVariable(std::string rhs);
     */

private:
//    std::unordered_map<StmtNum, Ent> print_ent;
//    std::unordered_map<StmtNum, std::unordered_set<Ent>> assign_ent;
//    std::unordered_map<StmtNum, std::unordered_set<Ent>> if_ent;
//    std::unordered_map<StmtNum, std::unordered_set<Ent>> while_ent;
    std::unordered_map<StmtNum, ProcedureName> call_callee;

    std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_ent;


    std::unordered_map<StmtNum, std::pair<ProcedureName, StmtType>> stmtNum_procName_stmtType;
    std::unordered_map<ProcedureName, std::unordered_map<StmtType, std::unordered_set<StmtNum>>> procName_stmtType_stmtNum;

//    std::unordered_map<ProcedureName, std::unordered_set<ProcedureName>> caller_callee; //for what
    //    std::unordered_map<ProcedureName, std::unordered_set<Ent>> procedure_entities; //good to have but can be derived
//    std::unordered_map<ProcedureName, std::unordered_set<ProcedureName>> caller_callee;

    //TODO: inverse table if we got time. Milestone 3?

    virtual std::vector<std::vector<std::string>> getUsesAllStatements(StmtType type);
    virtual std::vector<std::vector<std::string>> getUsesAllStatementsGivenProcedure(StmtType type, ProcedureName name);
    virtual std::vector<std::vector<std::string>> getUsesAllStatementsGivenEntity(StmtType type, Ent entity);

    /*
     * return type that are vectors needs to be of format: vector of {stmtNum, var}
     * assign a; print pn; stmt s (container statement); procedure p; call c (procedure call);
     *
     * Uses(int, int) => True / False
     * Uses(int, concreteVariable) => True / False
     * Uses(int, variable) => vector of vectors of {stmtNum, var}
     *
     * Uses(stmtRef, int) => True / False
     * Uses(stmtRef, concreteVariable) => True / False
     * Uses(stmtRef, variable) => vector of vectors of {stmtNum, var}
     *
     * Examples:
     * Select a such that Uses(a, "x")
     * Select a such that Uses(a, v)
     * Select v such that Uses(a, v)
     * Select pn such that Uses(pn, "x")
     * Select pn such that Uses(pn, v)
     * Select s such that Uses(s, "x")
     * Select s such that Uses(s, v)
     * Select p such that Uses(p, "x")
     * Select p such that Uses(p, v)
     * Select c such that Uses(c, "x")
     * Select c such that Uses(c, v)
     *
     * Select a such that Uses("procedure1", v) // and other variants ie pn, stmt, etc
     *
     * Select v such that Uses(1, 2) // where Uses(1, 2) is true
     *
     * Invalids:
     * ... Uses(_, _) => semantic error
     */

};
