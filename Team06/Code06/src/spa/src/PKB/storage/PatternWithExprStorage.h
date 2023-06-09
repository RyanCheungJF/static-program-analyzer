#pragma once

#include "../../../src/utils/AppConstants.h"
#include "../../SP/AST/Node/AssignStatement.h"
#include "../../SP/Parser.h"
#include "../../SP/Tokenizer.h"

struct hashFunction {
    size_t operator()(const std::pair<int, std::unique_ptr<Expression>>& x) const {
        std::size_t h2 = std::hash<double>{}(x.first);

        return h2;
    }
};

class PatternWithExprStorage {
public:
    void writePattern(Ent var, StmtNum num, std::unique_ptr<Expression> pointer);

    std::unordered_set<std::pair<StmtNum, std::unique_ptr<Expression>>, hashFunction>* getPatternWithLHS(Ent var);

    std::unordered_map<Ent, std::unordered_set<std::pair<StmtNum, std::unique_ptr<Expression>>, hashFunction>>*
    getAll();

private:
    std::unordered_map<Ent, std::unordered_set<std::pair<StmtNum, std::unique_ptr<Expression>>, hashFunction>>
        lhs_stmtNum_rhsPostfix;
};