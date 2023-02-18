#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include "../../../src/utils/AppConstants.h"

class ProcedureStorage {
public:
    virtual void writeProcedure(ProcName p, std::unordered_set<StmtNum> lines);
    virtual bool checkProcedure(ProcName p, StmtNum num);
    virtual std::unordered_set<StmtNum> getProcedureStatementNumbers(ProcName p);
    std::unordered_set<ProcName> getProcNames();
private:
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> proc_stmtNum;
    std::unordered_map<StmtNum, std::unordered_set<ProcName>> stmtNum_proc;
    std::unordered_set<ProcName> procedures;
};

