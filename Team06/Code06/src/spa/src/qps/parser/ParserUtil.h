#ifndef SPA_QPS_PARSERUTIL_H
#define SPA_QPS_PARSERUTIL_H
#include <string>
#include <bits/stdc++.h>
#include <vector>
#include "qps/QPSGrammarUtils.h"

using namespace std;

long findSuchThat(const vector<string> &wordList);
long findPattern(const vector<string> &wordList);
vector<string> stringToWordList(string s);

#endif // !SPA_QPS_PARSERUTIL_H
