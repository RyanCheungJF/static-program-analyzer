//
// Created by Faruq on 31/1/23.
//

#include "Query.h"

vector<string> Query::evaluate(ReadPKB& readPKB) {
    // I am going to assume here that since the object has been created it means
    // that the variables are correctly instantiated.
    QueryDB queryDb = QueryDB();
    QueryDB* queryDBPointer = &queryDb;
    Table emptyTable({}, {});

    evaluateComparison(queryDb, readPKB) && evaluatePattern(queryDb, readPKB) && evaluateRelationship(queryDb, readPKB);

    vector<string> res = queryDb.fetch(selectParameters, readPKB);
    return res;
}

bool Query::evaluateRelationship(QueryDB& queryDb, ReadPKB& readPKB) {
    for (int i = 0; i < this->relations.size(); i++) {
        shared_ptr<Relationship>& relation = this->relations.at(i);
        // Run an PKB API call for each relationship.
        // Taking the example of select s1 follows(s1, s2)
        vector<vector<string>> response = readPKB.findRelationship(relation);
        vector<Parameter> params = relation->getParameters();
        Table table(params, response);
        if (response.empty()) {
            queryDb.insertTable(QueryDB::emptyTable);
            return false;
        }
        // clauses that are just fixed ints or wild cards will just be
        // taken as true and not be inserted into the tableVec
        table.extractDesignEntities();
        if (!table.isEmptyTable()) {
            queryDb.insertTable(table);
        }
    }
    return true;
}

bool Query::evaluatePattern(QueryDB& queryDb, ReadPKB& readPKB) {
    for (Pattern& pattern : this->patterns) {
        // Run an PKB API call for each relationship.
        // Taking the example of select s1 follows(s1, s2)
        vector<vector<string>> response = readPKB.findPattern(pattern);
        Parameter* patternSyn = pattern.getPatternSyn();
        Parameter* entRef = pattern.getEntRef();
        vector<Parameter> headers{*patternSyn, *entRef};
        Table table(headers, response);
        if (response.empty()) {
            queryDb.insertTable(QueryDB::emptyTable);
            return false;
        }
        // This will remove wild cards and FIXED INT from the table.
        table.extractDesignEntities();
        if (!table.isEmptyTable()) {
            queryDb.insertTable(table);
        }
    }
    return true;
}

bool Query::evaluateComparison(QueryDB& queryDb, ReadPKB& readPKB) {
    for (Comparison& comparison : this->comparisons) {
        vector<vector<string>> response = readPKB.findWith(comparison);
        if (response.empty()) {
            queryDb.insertTable(QueryDB::emptyTable);
            return false;
        }
        Parameter leftParam = comparison.getLeftParam();
        Parameter rightParam = comparison.getRightParam();
        leftParam.updateAttributeType(AttributeType::NONE);
        rightParam.updateAttributeType(AttributeType::NONE);
        vector<Parameter> headers{leftParam, rightParam};
        Table table{headers, response};
        table.extractDesignEntities();
        if (!table.isEmptyTable()) {
            queryDb.insertTable(table);
        }
    }
    return true;
}

Query::Query() {
    isSelectTuple = false;
}

Query::Query(const Query& q) {
    relations = q.relations;
    selectParameters = q.selectParameters;
    patterns = q.patterns;
    comparisons = q.comparisons;
    isSelectTuple = q.isSelectTuple;
}

Query::Query(vector<Parameter>& ss, vector<shared_ptr<Relationship>>& rs, vector<Pattern>& ps, vector<Comparison>& cs,
             bool ist) {
    selectParameters = ss;
    relations = rs;
    patterns = ps;
    comparisons = cs;
    isSelectTuple = ist;
}

vector<Parameter*> Query::getAllUncheckedSynonyms() {
    vector<Parameter*> synonyms;
    for (int i = 0; i < selectParameters.size(); i++) {
        if (selectParameters.at(i).isUncheckedSynonym()) {
            synonyms.push_back(&selectParameters.at(i));
        }
    }
    for (int i = 0; i < relations.size(); i++) {
        vector<Parameter*> relSyns = (*relations.at(i)).getAllUncheckedSynonyms();
        for (int j = 0; j < relSyns.size(); j++) {
            synonyms.push_back(relSyns.at(j));
        }
    }
    for (int i = 0; i < patterns.size(); i++) {
        Parameter* entRef = patterns.at(i).getEntRef();
        Parameter* patternSyn = patterns.at(i).getPatternSyn();
        if (entRef->getType() == ParameterType::SYNONYM) {
            synonyms.push_back(entRef);
        }
        if (patternSyn->getType() == ParameterType::SYNONYM) {
            synonyms.push_back(patternSyn);
        }
    }
    for (int i = 0; i < comparisons.size(); i++) {
        vector<Parameter*> params = comparisons.at(i).getAllUncheckedSynonyms();
        for (int j = 0; j < params.size(); j++) {
            synonyms.push_back(params.at(j));
        }
    }
    return synonyms;
}

bool Query::validateAllParameters() {

    for (Parameter p : selectParameters) {
        if (!p.hasValidAttributeType()) {
            return false;
        }
        if (selectParameters.size() > 1 && p.getType() == ParameterType::BOOLEAN) {
            return false;
        }
    }

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

    for (Comparison c : comparisons) {
        if (!c.validateParams()) {
            return false;
        }
    }

    return true;
}

bool Query::booleanParamCheck() {
    return selectParameters.size() == 1 && selectParameters[0].getType() == ParameterType::BOOLEAN && isSelectTuple;
}

bool Query::operator==(const Query& q) const {
    if (q.relations.size() != relations.size()) {
        return false;
    }
    for (int i = 0; i < relations.size(); i++) {
        if (!(*(relations.at(i)) == *(q.relations.at(i)))) {
            return false;
        }
    }
    return selectParameters == q.selectParameters && patterns == q.patterns && comparisons == q.comparisons &&
           isSelectTuple == q.isSelectTuple;
}

void Query::updateEvalOrder() {
    if (relations.size() > 0) {
        for (int i = 0; i < relations.size(); i++) {
            relations.at(i)->calcPriority();
        }
        std::sort(relations.begin(), relations.end(), SharedPtrCompare::cmp<Relationship>);
    }
    if (patterns.size() > 0) {
        for (int i = 0; i < patterns.size(); i++) {
            patterns.at(i).calcPriority();
        }
        std::sort(patterns.begin(), patterns.end(), greater());
    }
    if (comparisons.size() > 0) {
        for (int i = 0; i < comparisons.size(); i++) {
            comparisons.at(i).calcPriority();
        }
        std::sort(comparisons.begin(), comparisons.end(), greater());
    }
}
