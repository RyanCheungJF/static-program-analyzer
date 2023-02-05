#include "../storage/StmtStmtRLStorage.h"
#include "../../qps/entities/Parameter.h"


class StmtStmtRLHandler {
public:
	std::vector<std::vector<std::string>> handle(StmtStmtRLStorage *storage, std::string param1, std::string param2);
};