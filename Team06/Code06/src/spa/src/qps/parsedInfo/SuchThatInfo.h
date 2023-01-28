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

	SuchThatInfo();
	SuchThatInfo(string, vector<string>);
	SuchThatInfo(const SuchThatInfo&);
};

#endif // !SPA_QPS_SUCHTHATINFO_H
