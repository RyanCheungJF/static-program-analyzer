#include "../storage/StmtStorage.h"
#include <unordered_set>
#include <vector>
#include <utility>


typedef std::string Stmt;
typedef int StmtNum;

class Statement {
public:
    void writeStatement(Stmt s, std::vector<StmtNum> lines);

    // check if given a statement type and statement line number, whether that statement line number is indeed
    // of that statement type
    bool checkStatement(Stmt stmt, StmtNum num);

    std::unordered_set<StmtNum> getStatementNumbers(Stmt stmt);
private:
    StmtStorage store;
};
