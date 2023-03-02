#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>

namespace unit_testing_utils {
    bool contains(std::vector<std::vector<std::string>> result, std::vector<std::string> item);
    bool equals(std::vector<std::string> expected, std::vector<std::string> actual);
    bool equals(std::vector<int> expected, std::vector<int> actual);
    bool equals(std::vector<int> expected, std::unordered_set<int> actual);
    bool equals(std::unordered_set<int> expected, std::unordered_set<int> actual);
    bool equals(std::unordered_set<std::string> expected, std::unordered_set<std::string> actual);
    bool equals(std::vector<std::vector<std::string>> expected, std::vector<std::vector<std::string>> actual);
    bool equals(std::vector<std::pair<std::string, std::string>> expected, std::vector<std::pair<std::string, std::string>> actual);
}