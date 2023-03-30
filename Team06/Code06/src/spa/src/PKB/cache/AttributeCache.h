#include "../../QPS/entities/Parameter.h"
#include "Cache.h"

struct attributeHash {
    std::size_t operator()(const shared_ptr<Parameter>& param) const {
        std::size_t h1 = std::hash<ParameterType>{}(param->getType());
        std::size_t h2 = std::hash<AttributeType>{}(param->getAttribute());

        return h1 ^ (h2 << 1);
    }
};

struct attributeEquals {
    bool operator()(const shared_ptr<Parameter>& param1, const shared_ptr<Parameter>& param2) const {
        bool check1 = param1->getType() == param2->getType();
        bool check2 = param1->getAttribute() == param2->getAttribute();

        return check1 && check2;
    }
};

class AttributeCache : public Cache<Parameter, std::vector<std::vector<std::string>>, attributeHash, attributeEquals> {
};
