#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../../src/utils/AppConstants.h"

template <typename T> class EntityStorage {
public:
    void writeEntity(StmtNum num, std::unordered_set<T> entities);
    bool checkEntity(T entity, StmtNum num);
    std::unordered_set<StmtNum> getEntityStmtNums(T e);
    std::unordered_set<T> getEntities(StmtNum num);
    std::unordered_set<T> getEntNames();

private:
    std::unordered_map<StmtNum, std::unordered_set<T>> stmtNum_ent;
    std::unordered_map<T, std::unordered_set<StmtNum>> ent_stmtNum;
    std::unordered_set<T> entNames;
};
