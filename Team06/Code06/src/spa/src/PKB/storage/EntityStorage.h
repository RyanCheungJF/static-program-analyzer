#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

typedef std::string Ent;
typedef int StmtNum;

class EntityStorage {
public:
    virtual void writeEntity(Ent e, std::vector<StmtNum> lines);
    virtual bool checkEntity(Ent e, StmtNum num);
    virtual std::unordered_set<StmtNum> getEntityStmtNums(Ent e);
    virtual std::unordered_set<std::string> getEntNames(Ent e);
private:
    std::unordered_map<Ent, std::unordered_set<StmtNum>> ent_stmtNum;
    std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_ent;
    std::unordered_map<Ent, std::unordered_set<std::string>> entToNameMap;
};
