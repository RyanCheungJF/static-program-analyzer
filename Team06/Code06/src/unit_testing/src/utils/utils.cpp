#include "utils.h"

bool unit_testing_utils::contains(std::vector<std::vector<std::string>> result, std::vector<std::string> item) {
	return find(result.begin(), result.end(), item) != result.end();
}
