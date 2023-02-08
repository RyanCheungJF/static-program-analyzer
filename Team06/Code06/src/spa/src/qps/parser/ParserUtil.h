#ifndef SPA_QPS_PARSERUTIL_H
#define SPA_QPS_PARSERUTIL_H
#include <string>
#include <vector>
#include <tuple>
#include <sstream>
#include "../QPSGrammarUtils.h"
#include "../../exceptions/Exception.h"

using namespace std;

long findSuchThat(const vector<string> &wordList);
long findPattern(const vector<string> &wordList);
vector<tuple<string, string, string>> extractParameters(string s);
tuple<string, size_t> extractSubStringUntilDelimiter(const string& original, int start, string delimiter);
vector<string> stringToWordList(string s);
vector<string> stringToWordListByDelimiter(string original, string delimiter);
string trim(const string&);
string removeCharFromString(string, char);

#endif // !SPA_QPS_PARSERUTIL_H
