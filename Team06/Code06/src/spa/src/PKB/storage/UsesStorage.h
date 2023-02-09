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

struct usesCallsHashFunction
{
    size_t operator()(const std::pair<std::string, int>& x) const
    {
        std::size_t h1 = std::hash<std::string>{}(x.first);
        std::size_t h2 = std::hash<double>{}(x.second);
        return h1 ^ h2;
    }
};

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

    // Select c such that Uses(c, v)
    // return {StmtNum, entity} or {StmtNum, procedureName} ?
    virtual std::vector<std::vector<std::string>> getUsesAllCallStatements_format1();
    virtual std::vector<std::vector<std::string>> getUsesAllCallStatements_format2();

    // Select c such that Uses("proc1", v) or Select pn such that Uses("proc1", _)
    // return {StmtNum, entity} or {StmtNum, calleeName} or {StmtNum, names of all procedure calls inside called procedure (recursive)} ?
    virtual std::vector<std::vector<std::string>> getUsesAllCallStatementsGivenProcedure_format1(ProcedureName name);
    virtual std::vector<std::vector<std::string>> getUsesAllCallStatementsGivenProcedure_format2(ProcedureName name);
    virtual std::vector<std::vector<std::string>> getUsesAllCallStatementsGivenProcedure_format3(ProcedureName name);

    // Select c such that Uses(c, "x")
    // return {StmtNum, entity} or {StmtNum, calleeName} ?
    virtual std::vector<std::vector<std::string>> getUsesAllCallStatementsGivenEntity_format1(Ent entity);
    virtual std::vector<std::vector<std::string>> getUsesAllCallStatementsGivenEntity_format2(Ent entity);

    // Select s such that Uses(s, v)
    virtual std::vector<std::vector<std::string>> getUsesAllStatements();

    // Select s such that Uses("proc1", v) or Select pn such that Uses("proc1", _)
    virtual std::vector<std::vector<std::string>> getUsesAllStatementsGivenProcedure(ProcedureName name);

    // Select s such that Uses(s, "x")
    virtual std::vector<std::vector<std::string>> getUsesAllStatementsGivenEntity(Ent entity);

    /*
    // Select p such that Uses(p, v)
    virtual std::vector<std::vector<std::string>> getUsesAllProcedures();

    // Select p such that Uses("proc1", v) or Select pn such that Uses("proc1", _)
    virtual std::vector<std::vector<std::string>> getUsesAllProceduresGivenProcedure(ProcedureName name);

    // Select p such that Uses(p, "x")
    virtual std::vector<std::vector<std::string>> getUsesAllProceduresGivenEntity(Ent e);
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
