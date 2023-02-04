#include "../storage/StmtStmtRLStorage.h"
#include "../../qps/entities/Parameter.h"


class StmtStmtRLHandler {
public:
	std::vector<std::pair<std::string, std::string>> handle(StmtStmtRLStorage *storage, std::string param1, std::string param2);
};