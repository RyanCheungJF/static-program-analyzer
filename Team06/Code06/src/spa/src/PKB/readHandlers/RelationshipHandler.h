#pragma once
#include "../../QPS/entities/Parameter.h"

class RelationshipHandler {
public:
    virtual std::vector<std::vector<std::string>> handle(Parameter& param1, Parameter& param2);
    void setIsTransitive(bool isTransitive);

protected:
    bool isTransitive;
};
