#include "../../QPS/entities/Parameter.h"
#include "Cache.h"

struct parameterHash {
    std::size_t operator()(shared_ptr<Parameter> const& param) const {
        std::size_t h1 = std::hash<ParameterType>{}(param->getType());
        std::size_t h2 = std::hash<std::string>{}(param->getValue());

        return h1 ^ (h2 << 1);
    }
};

struct parameterEquals {
    bool operator()(shared_ptr<Parameter> const& param1, shared_ptr<Parameter> const& param2) const {
        bool check1 = param1->getType() == param2->getType();
        bool check2 = param1->getValue() == param2->getValue();

        return check1 && check2;
    }
};

class ParameterCache : public Cache<Parameter, std::vector<std::string>, parameterHash, parameterEquals> {};