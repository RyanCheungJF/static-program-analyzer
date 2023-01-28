#ifndef SPA_QPS_SYNONYM_H
#define SPA_QPS_SYNONYM_H
#include <string>
using namespace std;

class Synonym {
public:
	string type;
	string name;

	Synonym();
	Synonym(const Synonym& s);
	Synonym(string, string);
};