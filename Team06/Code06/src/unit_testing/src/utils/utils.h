#include <vector>
#include <string>
#include <unordered_set>

namespace unit_testing_utils {
    bool contains(std::vector<std::vector<std::string>> result, std::vector<std::string> item);
    bool equals(std::vector<std::string> expected, std::vector<std::string> actual);
    bool equals(std::vector<int> expected, std::vector<int> actual);
    bool equals(std::vector<int> expected, std::unordered_set<int> actual);
    bool equals(std::unordered_set<int> expected, std::unordered_set<int> actual);
    bool equals(std::vector<std::vector<std::string>> expected, std::vector<std::vector<std::string>> actual);
}