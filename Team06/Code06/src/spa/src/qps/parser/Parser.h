#ifndef SPA_PARSER_H
#define SPA_PARSER_H
#include <string>
using namespace std;

template <class T> class Parser {
public:
	virtual T parse(string item) = 0;
};

#endif // SPA_PARSER_H