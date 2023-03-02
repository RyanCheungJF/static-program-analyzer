#include "../../../src/utils/AppConstants.h"
#include "../../qps/entities/Parameter.h"
#include "../storage/ModifiesUsesStorage.h"
#include "../storage/StmtStorage.h"
#include <memory>

class ModifiesUsesHandler {
public:
  ModifiesUsesHandler(std::shared_ptr<ModifiesUsesStorage> rlStorage, std::shared_ptr<StmtStorage> stmtStorage);
  std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2);

private:
  std::shared_ptr<ModifiesUsesStorage> rlStorage;
  std::shared_ptr<StmtStorage> stmtStorage;

  // e.g. Uses(2, "x")
  std::vector<std::vector<std::string>> handleIntVar(Parameter param1, Parameter param2);

  // e.g. Uses(2, v) OR Uses(2, _)
  std::vector<std::vector<std::string>> handleIntWildcard(Parameter fixedIntParam);

  // e.g. Uses("main", "x")
  std::vector<std::vector<std::string>> handleProcVar(Parameter param1, Parameter param2);

  // e.g. Uses("main", v) OR Uses("main", _)
  std::vector<std::vector<std::string>> handleProcWildcard(Parameter fixedProcParam);

  // e.g. Uses(s, "x")
  std::vector<std::vector<std::string>> handleStmtSynVar(Parameter param1, Parameter param2);

  // e.g. Uses(s, v) OR Uses(s, _)
  std::vector<std::vector<std::string>> handleStmtSynWildcard(Parameter param1);

  // e.g. Uses(p, "x")
  std::vector<std::vector<std::string>> handleProcSynVar(Parameter fixedVarParam);

  // .e.g Uses(p, v) OR Uses(p, _)
  std::vector<std::vector<std::string>> handleProcSynWildcard();
};