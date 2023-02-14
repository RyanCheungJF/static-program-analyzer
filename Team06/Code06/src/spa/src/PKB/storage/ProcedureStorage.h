#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

typedef std::string Proc;
typedef int StmtNum;

class ProcedureStorage {
public:
    virtual void writeProcedure(Proc p, std::unordered_set<StmtNum> lines);
    virtual void writeProcedure(Proc p, StmtNum line);
    virtual bool checkProcedure(Proc p, StmtNum num);
    virtual std::unordered_set<StmtNum> getProcedureStatementNumbers(Proc p);
    std::unordered_set<Proc> getProcNames();
private:
    std::unordered_map<Proc, std::unordered_set<StmtNum>> proc_stmtNum;
    std::unordered_map<StmtNum, std::unordered_set<Proc>> stmtNum_proc;
    std::unordered_set<Proc> procNames;
};

