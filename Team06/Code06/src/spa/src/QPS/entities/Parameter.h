#ifndef SPA_QPS_PARAMETER_H
#define SPA_QPS_PARAMETER_H
#include "../../../src/utils/AppConstants.h"
#include "exceptions/InternalException.h"
#include "QPS/QPSGrammarUtils.h"
#include <string>
#include <unordered_map>
using namespace std;

enum class ParameterType {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE,
  SYNONYM,
  WILDCARD,
  FIXED_INT,
  FIXED_STRING,
  FIXED_STRING_WITH_WILDCARD,
  UNKNOWN
};

class Parameter {
public:
  string getValue();
  ParameterType getType() const;
  Parameter(string, string);
  Parameter(string, ParameterType);
  Parameter(const Parameter &);
  Parameter();
  static bool isSyntacticEntityRef(Parameter &);
  static bool isEntityRef(Parameter &);
  static bool isSyntacticStatementRef(Parameter &);
  static bool isStatementRef(Parameter &);
  static bool isProcedure(Parameter &);
  static bool isDsgEntity(Parameter &);
  static bool isPatternSyn(Parameter &);
  bool isUncheckedSynonym();
  void updateSynonymType(ParameterType);
  string getTypeString() const;
  bool isEqualTo(Parameter);
  bool operator==(const Parameter &) const;

  static ParameterType guessParameterType(string);

private:
  const static unordered_map<string, ParameterType> stringToTypeMap;
  static ParameterType stringToType(string);
  string value;
  ParameterType type;
};

#endif