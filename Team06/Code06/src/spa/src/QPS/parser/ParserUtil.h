#ifndef SPA_QPS_PARSERUTIL_H
#define SPA_QPS_PARSERUTIL_H
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "../../exceptions/Exception.h"
#include "../QPSGrammarUtils.h"
#include "exceptions/InternalException.h"
#include "exceptions/SyntaxException.h"
using namespace std;

vector<int> findSuchThat(const vector<string>& wordList);
vector<int> findPattern(const vector<string>& wordList);
vector<int> findAnds(const vector<string>& wordList, int start, int end);
tuple<string, vector<string>> extractParameters(string s, string containerStart, string containerEnd, string delimiter);
tuple<string, size_t, bool> extractSubStringUntilDelimiter(const string& original, int start, string delimiter);
vector<string> stringToWordList(string s);
vector<string> stringToWordListByDelimiter(string original, string delimiter);
string trim(const string&);
string removeCharFromString(string, char);

#endif // !SPA_QPS_PARSERUTIL_H
