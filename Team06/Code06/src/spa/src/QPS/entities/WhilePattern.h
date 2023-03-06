#ifndef SPA_WHILEPATTERN_H
#define SPA_WHILEPATTERN_H
#include "Pattern.h"

class WhilePattern : public Pattern {
public:
    WhilePattern(Parameter ps, Parameter er, string p);
    bool validateParams();
};

#endif // SPA_WHILEPATTERN_H
