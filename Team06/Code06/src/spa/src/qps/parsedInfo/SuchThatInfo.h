#ifndef SPA_QPS_SUCHTHATINFO_H
#define SPA_QPS_SUCHTHATINFO_H
#include <string>
#include <vector>
#include "../entities/Relationship.h"
using namespace std;

class SuchThatInfo {
	Relationship rel;
public:
	SuchThatInfo();
	SuchThatInfo(Relationship);
	SuchThatInfo(const SuchThatInfo&);
};

#endif // !SPA_QPS_SUCHTHATINFO_H
