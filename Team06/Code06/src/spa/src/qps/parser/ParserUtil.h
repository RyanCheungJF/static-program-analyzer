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
tuple<string, size_t> extractSubStringUntilDelimiter(const string& original, int start, string delimiter);
vector<string> stringToWordList(string s);
vector<string> stringToWordListByDelimiter(string original, string delimiter);

#endif // !SPA_QPS_PARSERUTIL_H
