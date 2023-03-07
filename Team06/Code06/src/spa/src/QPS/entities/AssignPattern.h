#ifndef SPA_ASSIGNPATTERN_H
#define SPA_ASSIGNPATTERN_H
#include "Pattern.h"

class AssignPattern : public Pattern {
public:
    AssignPattern(Parameter ps, Parameter er, string p);
    bool validateParams();
};

#endif // SPA_ASSIGNPATTERN_H
