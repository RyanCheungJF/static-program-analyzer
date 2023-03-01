#include "../../../src/utils/AppConstants.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>

class PatternStorage {
public:
  void writePattern(StmtNum stmtNum, std::unordered_set<Ent> variables);
  std::unordered_set<StmtNum> getStmtNums(Ent ent);
  std::pair<std::vector<StmtNum>, std::vector<Ent>> getAllPairs();

private:
  std::unordered_map<Ent, std::unordered_set<StmtNum>> varsToStmtNumMap;
};