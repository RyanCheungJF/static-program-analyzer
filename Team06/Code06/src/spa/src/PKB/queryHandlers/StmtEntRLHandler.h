#include "../storage/StmtEntRLStorage.h"
#include <memory>


class StmtEntRLHandler {
public:
	std::vector<std::vector<std::string>> handle(std::shared_ptr<StmtEntRLStorage> storage);
};