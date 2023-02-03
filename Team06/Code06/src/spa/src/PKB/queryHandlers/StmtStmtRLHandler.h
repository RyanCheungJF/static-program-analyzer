#include "../storage/StmtStmtRLStorage.h"


class StmtStmtRLHandler {
public:
	std::vector<std::pair<std::string, std::string>> handle(StmtStmtRLStorage *storage);
};