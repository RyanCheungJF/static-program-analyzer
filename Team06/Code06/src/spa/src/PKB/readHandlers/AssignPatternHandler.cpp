#include "AssignPatternHandler.h"

AssignPatternHandler::AssignPatternHandler(std::shared_ptr<PatternStorage> patternStorage) {
    this->patternStorage = patternStorage;
}

bool isSameTree(Expression* expected, Expression* actual) {
        // if they are both constants, check they have the same value
    if (dynamic_cast<Constant*>(expected) && dynamic_cast<Constant*>(actual)) {
        auto expectedValue = dynamic_cast<Constant*>(expected)->value;
        auto actualValue = dynamic_cast<Constant*>(actual)->value;
        return expectedValue == actualValue;
    }
    // if they are both variables, check they have the same value
    else if (dynamic_cast<Variable*>(expected) && dynamic_cast<Variable*> (actual)) {
        auto expectedName = dynamic_cast<Variable*>(expected)->name;
        auto actualName = dynamic_cast<Variable*>(actual)->name;
        return expectedName == actualName;
    }
    // if they are both math expressions, recursively check that left and right are same type of expressions
    else if (dynamic_cast<MathExpression*>(expected) && dynamic_cast<MathExpression*>(actual)) {
        auto expectedExpression = dynamic_cast<MathExpression*>(expected);
        auto actualExpression = dynamic_cast<MathExpression*>(actual);

        bool isSameLeftExpression = isSameTree(expectedExpression->lhs.get(), actualExpression->lhs.get());
        bool isSameRightExpression = isSameTree(expectedExpression->rhs.get(), actualExpression->rhs.get());
        bool isSameOp = (expectedExpression->mathOperator == actualExpression->mathOperator);
        return (isSameLeftExpression && isSameRightExpression && isSameOp);
    }
    return false;
}

bool isSubTree(Expression* subTreeExpression, Expression* treeExpression) {
    // check if they are the same tree first
    if (isSameTree(subTreeExpression, treeExpression)) {
        return true;
    }

    auto tree = treeExpression;

    if (dynamic_cast<MathExpression*>(tree)) {
        auto t = dynamic_cast<MathExpression*>(tree);
        bool isLeftSubtree = isSubTree(subTreeExpression, t->lhs.get());
        bool isRightSubtree = isSubTree(std::move(subTreeExpression), t->rhs.get());
        return isLeftSubtree || isRightSubtree;
    }
    // if my actual tree node is not a math node,
    // this means my actual tree node is a variable or a constant, and hence has no children
    // if they are not the same tree (checked above), that means it is not a subtree
    return false;
}

std::vector<std::vector<std::string>> AssignPatternHandler::handleVarWildcard(std::string lhs) {
    std::vector<std::vector<std::string>> res;

    std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction>* pointer = patternStorage->getPatternWithLHS(lhs);
    if (pointer == NULL) return res;

    for (const auto& pair : *pointer) {
        res.push_back({ std::to_string(pair.first), lhs });
    }
    return res;
}

std::vector<std::vector<std::string>> AssignPatternHandler::handleVarPattern(std::string lhs, std::string rhs, bool (*checkTree)(Expression*, Expression*)) {
    std::unique_ptr<Expression> expected = pkb_utils::buildSubtree(rhs);
    std::vector<std::vector<std::string>> res;

    std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction>* pointer = patternStorage->getPatternWithLHS(lhs);
    if (pointer == NULL) return res;

    for (const auto& pair : *pointer) {
        Expression* actual = pair.second.get();
        if (checkTree(expected.get(), actual)) {
            res.push_back({ std::to_string(pair.first), lhs });
        }
    }
    return res;
}

std::vector<std::vector<std::string>> AssignPatternHandler::handleWildcardWildcard() {
    std::vector<std::vector<std::string>> res;

    for (auto const& row : *patternStorage->getAll()) {
        for (const auto& pair : row.second) {
            Expression* actual = pair.second.get();
            std::vector<std::string> curr;
            curr.push_back(std::to_string(pair.first));
            curr.push_back(row.first);
            res.push_back(curr);
        }
    }
    return res;
}

std::vector<std::vector<std::string>> AssignPatternHandler::handleSynPattern(std::string rhs, bool (*checkTree)(Expression*, Expression*)) {
    std::vector<std::vector<std::string>> res;
    std::unique_ptr<Expression> expected = std::move(pkb_utils::buildSubtree(rhs));

    for (auto const& row : *patternStorage->getAll()) {
        for (const auto& pair : row.second) {
            Expression* actual = pair.second.get();
            std::vector<std::string> curr;
            if (checkTree(expected.get(), actual)) {
                curr.push_back(std::to_string(pair.first));
                curr.push_back(row.first);
                res.push_back(curr);
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> AssignPatternHandler::handleWildcardPattern(std::string rhs, bool (*checkTree)(Expression*, Expression*)) {
    std::unique_ptr<Expression> expected = std::move(pkb_utils::buildSubtree(rhs));
    std::vector<std::vector<std::string>> res;

    for (auto const& row : *patternStorage->getAll()) {
        for (const auto& pair : row.second) {
            Expression* actual = pair.second.get();
            std::vector<std::string> curr;
            if (checkTree(expected.get(), actual)) {
                curr.push_back(std::to_string(pair.first));
                curr.push_back(row.first);
                res.push_back(curr);
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> AssignPatternHandler::handle(Pattern p) {
    Parameter* lhs = p.getEntRef();
    std::string rhs = p.pattern;
    std::string lhsString = lhs->getValue();
    ParameterType lhsType = lhs->getType();
    bool leftWildcard = false;
    bool rightWildcard = false;

    if (rhs != "_") {
        leftWildcard = rhs[0] == '_';
        rightWildcard = rhs[rhs.length() - 1] == '_';
    }

    if (leftWildcard && rightWildcard) rhs = rhs.substr(1, rhs.length() - 2);


    if (lhsType == ParameterType::FIXED_STRING) {
        if (rhs == "_") {
            return handleVarWildcard(lhsString);
        }
        else if (leftWildcard && rightWildcard) {
            return handleVarPattern(lhsString, rhs, isSubTree);
        }
        else {
            return handleVarPattern(lhsString, rhs, isSameTree);
        }
    }
    else if (lhsType == ParameterType::VARIABLE) {
        if (rhs == "_") {
            return handleWildcardWildcard();
        }
        else if (leftWildcard && rightWildcard) {
            return handleSynPattern(rhs, isSubTree);
        }
        else {
            return handleSynPattern(rhs, isSameTree);
        }
    }
    else {
        if (rhs == "_") {
            return handleWildcardWildcard();
        }
        else if (leftWildcard && rightWildcard) {
            return handleWildcardPattern(rhs, isSubTree);
        }
        else {
            return handleWildcardPattern(rhs, isSameTree);
        }
    }
}