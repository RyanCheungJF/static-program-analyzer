#include "../../../src/utils/AppConstants.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class EntityStorage {
public:
  void writeEntity(StmtNum num, std::unordered_set<Ent> entities);
  bool checkEntity(Ent e, StmtNum num);
  std::unordered_set<StmtNum> getEntityStmtNums(Ent e);
  std::unordered_set<Ent> getEntities(StmtNum num);
  std::unordered_set<Ent> getEntNames();

private:
  std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_ent;
  std::unordered_map<Ent, std::unordered_set<StmtNum>> ent_stmtNum;
  std::unordered_set<Ent> entNames;
};
