#ifndef SPA_QPS_SUCHTHATINFO_H
#define SPA_QPS_SUCHTHATINFO_H
#include <string>
#include <vector>
using namespace std;

class SuchThatInfo {
	//TODO: change to a relationship object
	string relationship;
	//TODO: change to actual entities rather than strings
	vector<string> params;

	SuchThatInfo(string, vector<string>);
};

#endif // !SPA_QPS_SUCHTHATINFO_H
