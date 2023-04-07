#pragma once
#include "../../QPS/entities/Pattern.h"

class PatternHandler {
public:
    virtual std::vector<std::vector<std::string>> handle(Pattern& p);
};
