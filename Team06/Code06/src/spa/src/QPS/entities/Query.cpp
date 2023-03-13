//
// Created by Faruq on 31/1/23.
//

#include "Query.h"

vector<string> Query::evaluate(ReadPKB &readPKB) {
  // I am going to assume here that since the object has been created it means
  // that the variables are correctly instantiated.
  // TODO : refactor this into its individual components. function getting too long
  QueryDB queryDb = QueryDB();
  vector<string> emptyVec = {};
  vector<string> falseVec = {"false"};
  vector<string> trueVec = {"true"};
  for (shared_ptr<Relationship> relation : relations) {
    // Run an PKB API call for each relationship.
    // Taking the example of select s1 follows(s1, s2)
    vector<vector<string>> response = readPKB.findRelationship(relation);
    vector<Parameter> params = relation->getParameters();
    Table table(params, response);
    if (response.empty()) {
        return selectParameters.size() == 1
               && selectParameters[0].getType()==ParameterType::BOOLEAN
               ? falseVec : emptyVec;
    }
    // This will remove wild cards and FIXED INT from the table.
    table = table.extractDesignEntities();
    queryDb.insertTable(table);
  }

  for (Pattern pattern : patterns) {
    // Run an PKB API call for each relationship.
    // Taking the example of select s1 follows(s1, s2)
    vector<vector<string>> response = readPKB.findPattern(pattern);
    Parameter *patternSyn = pattern.getPatternSyn();
    Parameter *entRef = pattern.getEntRef();
    vector<Parameter> headers{*patternSyn, *entRef};
    Table table(headers, response);
    if (response.empty()) {
        return selectParameters.size() == 1
        && selectParameters[0].getType()==ParameterType::BOOLEAN
        ? falseVec : emptyVec;
    }
    // This will remove wild cards and FIXED INT from the table.
    table = table.extractDesignEntities();
    queryDb.insertTable(table);
  }
  // TODO: Add separate logic for BOOLEAN because there is no need to store any table if return value is boolean.
    return selectParameters.size() == 1
           && selectParameters[0].getType()==ParameterType::BOOLEAN
           ? trueVec : queryDb.fetch(selectParameters, readPKB);
}

Query::Query() {}

Query::Query(const Query &q) {
  for (int i = 0; i < q.relations.size(); i++) {
  }
  relations = q.relations;
  selectParameters = q.selectParameters;
  patterns = q.patterns;
}

Query::Query(vector<Parameter> &ss, vector<shared_ptr<Relationship>> &rs,
             vector<Pattern> &ps) {
  selectParameters = ss;
  relations = rs;
  patterns = ps;
}

vector<Parameter *> Query::getAllUncheckedSynonyms() {
  vector<Parameter *> synonyms;
  for (int i = 0; i < selectParameters.size(); i++) {
    if (selectParameters.at(i).isUncheckedSynonym()) {
      synonyms.push_back(&selectParameters.at(i));
    }
  }
  for (int i = 0; i < relations.size(); i++) {
    vector<Parameter *> relSyns = (*relations.at(i)).getAllUncheckedSynonyms();
    for (int j = 0; j < relSyns.size(); j++) {
      synonyms.push_back(relSyns.at(j));
    }
  }
  for (int i = 0; i < patterns.size(); i++) {
    Parameter *entRef = patterns.at(i).getEntRef();
    Parameter *patternSyn = patterns.at(i).getPatternSyn();
    if (entRef->getType() == ParameterType::SYNONYM) {
      synonyms.push_back(entRef);
    }
    if (patternSyn->getType() == ParameterType::SYNONYM) {
      synonyms.push_back(patternSyn);
    }
  }
  return synonyms;
}

bool Query::validateAllParameters() {
  for (Pattern p : patterns) {
    if (!p.validateParams()) {
      return false;
    }
  }

  for (shared_ptr<Relationship> r : relations) {
    if (!(*r).validateParams()) {
      return false;
    }
  }
  return true;
}
