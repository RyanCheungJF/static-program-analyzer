#include "../storage/StmtStmtRLStorage.h"
#include "../../qps/entities/Parameter.h"
#include "../storage/StmtStorage.h"
#include <algorithm>


class StmtStmtRLHandler {
public:
	std::vector<std::vector<std::string>> handle(StmtStmtRLStorage* rlStorage, StmtStorage* statementStorage, Parameter param1, Parameter param2);
};