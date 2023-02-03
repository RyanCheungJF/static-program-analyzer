#pragma once
#include <string>
#include <vector>
#include <utility>
#include "../storage/Storage.h"

class RLQueryHandler {
public:
	virtual std::vector<std::pair<std::string, std::string>> handle(Storage *storage);
};