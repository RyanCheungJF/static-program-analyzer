#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

typedef std::string Stmt;
typedef int StmtNum;

class StmtStorage {
public:
    virtual void writeStmt(Stmt s, StmtNum line);
    virtual void writeStmt(Stmt s, std::vector<StmtNum> lines);
    virtual std::unordered_set<StmtNum> getStmtNums(Stmt s);
private:
    std::unordered_map<Stmt, std::unordered_set<StmtNum>> stmt_stmtNum;
    std::unordered_map<StmtNum, std::unordered_set<Stmt>> stmtNum_stmt;
};
