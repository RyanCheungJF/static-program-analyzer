#include "../../QPS/entities/Parameter.h"
#include "Cache.h"

struct parameterHash {
    std::size_t operator()(const shared_ptr<Parameter>& param) const {
        std::size_t h1 = std::hash<ParameterType>{}(param->getType());

        return h1;
    }
};

struct parameterEquals {
    bool operator()(const shared_ptr<Parameter>& param1, const shared_ptr<Parameter>& param2) const {
        bool check1 = param1->getType() == param2->getType();

        return check1;
    }
};

class ParameterCache : public Cache<Parameter, std::vector<std::string>, parameterHash, parameterEquals> {};
