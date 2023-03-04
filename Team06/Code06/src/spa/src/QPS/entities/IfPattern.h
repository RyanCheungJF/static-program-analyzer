#ifndef SPA_IFPATTERN_H
#define SPA_IFPATTERN_H
#include "Pattern.h"

using namespace std;

class IfPattern : public Pattern {
public:
  IfPattern(Parameter ps, Parameter er, string p, Parameter ip);
  bool validateParams();
};

#endif // SPA_IFPATTERN_H
