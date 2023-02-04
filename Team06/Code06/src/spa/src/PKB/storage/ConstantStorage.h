#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

typedef std::string Const;
typedef int StmtNum;

class ConstantStorage {
public:
    virtual void writeConstant(Const c, StmtNum line);
    virtual bool checkConstant(Const c, StmtNum num);
    virtual std::unordered_set<StmtNum> getConstantStmtNums(Const c);
private:
    std::unordered_map<Const, std::unordered_set<StmtNum>> constant_stmtNum;
    std::unordered_map<StmtNum, std::unordered_set<Const>> stmtNum_constant;
};
