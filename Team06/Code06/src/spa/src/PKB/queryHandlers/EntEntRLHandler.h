#include "../storage/EntEntRLStorage.h"
#include <memory>

class EntEntRLHandler {
public:
	std::vector<std::vector<std::string>> handle(std::shared_ptr<EntEntRLStorage> storage);
};