#include "utils.h"

bool unit_testing_utils::contains(std::vector<std::vector<std::string>> result, std::vector<std::string> item) {
  return find(result.begin(), result.end(), item) != result.end();
}

bool unit_testing_utils::equals(std::vector<std::string> expected, std::vector<std::string> actual) {
  std::sort(actual.begin(), actual.end());
  std::sort(expected.begin(), expected.end());
  return expected == actual;
}

bool unit_testing_utils::equals(std::vector<int> expected, std::vector<int> actual) {
  std::sort(actual.begin(), actual.end());
  std::sort(expected.begin(), expected.end());
  return expected == actual;
}

bool unit_testing_utils::equals(std::vector<int> expected, std::unordered_set<int> res) {
  std::vector<int> actual;
  for (auto i : res) {
    actual.push_back(i);
  }
  std::sort(actual.begin(), actual.end());
  std::sort(expected.begin(), expected.end());
  return expected == actual;
}

bool unit_testing_utils::equals(std::unordered_set<int> param1, std::unordered_set<int> param2) {
  std::vector<int> actual;
  for (auto i : param2) {
    actual.push_back(i);
  }

  std::vector<int> expected;
  for (auto i : param1) {
    expected.push_back(i);
  }

  std::sort(actual.begin(), actual.end());
  std::sort(expected.begin(), expected.end());
  return expected == actual;
}

bool unit_testing_utils::equals(std::vector<std::vector<std::string>> expected,
                                std::vector<std::vector<std::string>> actual) {
  std::sort(actual.begin(), actual.end());
  std::sort(expected.begin(), expected.end());
  return expected == actual;
}
