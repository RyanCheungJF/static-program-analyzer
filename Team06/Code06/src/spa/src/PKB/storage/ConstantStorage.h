#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

typedef std::string Constant;
typedef int StmtNum;

class ConstantStorage {
public:
    virtual void writeConstant(Constant c, StmtNum line);
    virtual bool checkConstant(Constant c, StmtNum num);
    virtual std::unordered_set<StmtNum> getConstantStmtNums(Constant c);
private:
    std::unordered_map<Constant, std::unordered_set<StmtNum>> constant_stmtNum;
    std::unordered_map<StmtNum, std::unordered_set<Constant>> stmtNum_constant;
};
