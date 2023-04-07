#ifndef SPQ_QPS_PATTERN_H
#define SPQ_QPS_PATTERN_H
#include "../../exceptions/SyntaxException.h"
#include "../syntaxValidator/SyntaxValidator.h"
#include "Parameter.h"
using namespace std;

class Pattern {
public:
    Parameter patternSyn;
    Parameter entRef;
    vector<string> exprSpecs;
    double evalPriority;
    Pattern();
    Pattern(const Pattern&);
    Pattern(Parameter, Parameter, vector<string>&);
    static Pattern makePattern(Parameter, Parameter, vector<string>&);
    vector<Parameter*> getAllUncheckedSynonyms();
    Parameter getPatternSyn() const;
    Parameter getEntRef() const;
    vector<string> getExprSpecs()const;
    bool validateParams();
    ParameterType getPatternType() const;
    ParameterType getEntRefType() const;
    std::string getEntRefValue() const;
    bool operator==(const Pattern&) const;
    bool operator>(const Pattern&) const;
    bool operator<(const Pattern&) const;
    double getPriority();
    double calcPriority();

private:
    static const unordered_map<int, shared_ptr<SyntaxValidator<Pattern>>> paramCountToValidatorMap;
};

#endif // SPQ_QPS_PATTERN_H