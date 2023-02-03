#include "RLQueryHandler.h"
#include "../storage/Storage.h"


class StmtStmtRLQueryHandler : public RLQueryHandler{
public:
	std::vector<std::pair<std::string, std::string>> handle(Storage *storage);
};