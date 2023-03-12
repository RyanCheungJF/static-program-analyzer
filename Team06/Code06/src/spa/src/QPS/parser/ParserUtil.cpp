#include "ParserUtil.h"

/*
returns index of "such" which is followed immediately by a "that" in the word
list
 */

const string WHITESPACE = " \n\r\t\f\v";
const string SUCH = "such";
const string THAT = "that";
const string PATTERN = "pattern";
const string AND = "and";

vector<int> findSuchThat(const vector<string> &wordList) {
  vector<int> suchThatStarts;
  for (int i = 0; i < wordList.size(); i++) {
    if (SUCH != wordList[i]) {
      continue;
    }
    if (i == wordList.size() - 1) {
      continue;
    }
    if (THAT != wordList[i + 1]) {
      continue;
    }
    suchThatStarts.push_back(i);
  }

  return suchThatStarts;
}

vector<int> findPattern(const vector<string> &wordList) {
  vector<int> patternStarts;
  for (int i = 0; i < wordList.size(); i++) {
    if (PATTERN != wordList[i]) {
      continue;
    }
    if (i == wordList.size() - 1) {
      continue;
    }
    if (!startsWithLetter(wordList[i + 1])) {
      continue;
    }
    patternStarts.push_back(i);
  }
  return patternStarts;
}

vector<int> findAnds(const vector<string> &wordList, int start, int end) {
  vector<int> ands;
  for (int i = start; i < end; i++) {
    if (AND != wordList[i]) {
      continue;
    }
    if (i == wordList.size() - 1) {
      continue;
    }
    ands.push_back(i);
  }
  return ands;
}

// assumes the string is of the correct form:
// ^.*{containerStart}.*{containerEnd}$
tuple<string, vector<string>> extractParameters(string s, string containerStart,
                                                string containerEnd,
                                                string delimiter) {
  tuple<string, vector<string>> res;
  int endOfString = s.size();
  int curIndex = 0;
  string outerParam = "";
  vector<string> innerParams;
  bool found = false;
  tie(outerParam, curIndex, found) =
      extractSubStringUntilDelimiter(s, curIndex, containerStart);
  if (!found) {
    throw InternalException("ParserUtil.extractParameters: containerStart not found");
  }
  string innerParamsString =
      s.substr(curIndex, endOfString - curIndex - containerEnd.size());
  endOfString = innerParamsString.size();
  curIndex = 0;
  while (curIndex < endOfString) {
    string curParam;
    tie(curParam, curIndex, found) = extractSubStringUntilDelimiter(innerParamsString, curIndex, delimiter);
    if (found && curIndex == innerParamsString.size()) {
      //end of string with delimiter at the end
      throw SyntaxException();
    }
    innerParams.push_back(curParam);
  }
  res = {outerParam, innerParams};
  return res;
}

string removeCharFromString(string s, char c) {
  s.erase(remove(s.begin(), s.end(), c), s.end());
  return s;
}

tuple<string, size_t, bool> extractSubStringUntilDelimiter(const string &original,
                                                     int start,
                                                     string delimiter) {
  if (delimiter == "") {
    throw InternalException(
        "Error: ParserUtils.extractSubStringUntilDelimiter bad delimiter");
  }
  if (start < 0 || start >= original.size()) {
    throw InternalException(
        "Error: ParserUtils.extractSubStringUntilDelimiter bad start");
  }
  size_t end = original.find(delimiter, start);
  if (end == string::npos) {
    return tuple<string, int, bool>(original.substr(start, original.size()),
                              original.size(), false);
  }
  size_t length = end - start;
  string substr = original.substr(start, length);
  return tuple<string, int, bool>(substr, end + delimiter.size(), true);
}

vector<string> stringToWordList(string s) {
  vector<string> wordList;
  stringstream ss(s);
  string word;
  while (ss >> word) {
    wordList.push_back(word);
  }
  return wordList;
}

string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string trim(const std::string &s) { return rtrim(ltrim(s)); }

vector<string> stringToWordListByDelimiter(string original, string delimiter) {
  vector<string> wordList;
  size_t length = original.length();
  while (length != 0) {
    size_t index = original.find(delimiter);
    if (index == string::npos) {
      wordList.push_back(trim(original));
      break;
    }
    string word = original.substr(0, index);
    wordList.push_back(trim(word));
    if (index + 1 < length) {
      original = original.substr(index + 1, length);
      length = original.length();
    } else {
      break;
    }
  }
  return wordList;
}
