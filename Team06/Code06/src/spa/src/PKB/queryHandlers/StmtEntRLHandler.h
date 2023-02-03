#include "../storage/StmtEntRLStorage.h"


class StmtEntRLHandler {
public:
	std::vector<std::pair<std::string, std::string>> handle(StmtEntRLStorage* storage);
};