#include "PatternStorage.h"
#include <string>
#include <unordered_map>


bool isSameTree(Expression* expected, Expression* actual) {
//    auto expected = expectedExpression.get();
//    auto actual = actualExpression->get();

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
//        bool isSameLeftExpression = isSameTree(std::move(expectedExpression->lhs), std::move(actualExpression->lhs));
//        bool isSameRightExpression = isSameTree(std::move(expectedExpression->rhs), std::move(actualExpression->rhs));

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

void PatternStorage::writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> ptr) {
    lhs_stmtNum_rhsPostfix[lhs].insert(std::make_pair(num, std::move(ptr)));
    return;
}

std::vector<std::pair<std::string, int>> PatternStorage::getLHSAndStmtNum() {
    std::vector<std::pair<std::string, int>> res;

    for (auto const& i : lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            Expression* actual = p.second.get();
            std::pair<std::string, int> curr = std::make_pair(i.first, p.first);
            res.push_back(curr);
        }
    }
    return res;
}

std::vector<StmtNum> PatternStorage::interpretQuery(QueryStub pq) {
    std::string lhs = pq.lhs;
    std::string pattern = pq.pattern;
    std::vector<StmtNum> res;

    bool leftWildcard = pattern[0] == '_';
    bool rightWildcard = pattern[pattern.length() - 1] == '_';

    std::stringstream ss;
    for (int i = 0; i < pattern.length(); i++) {
        char curr = pattern[i];
        if (curr != '_') {
            ss << curr;
        }
    }

    std::string rhs = ss.str();
    if (lhs == "_") {
        if (leftWildcard && rightWildcard) {
            std::cout << "hitting getMatchingLHSWildcardRHSBothWildcard ie pattern(_, _\"v\"_) for " << lhs << " = " << rhs << "\n";
            return getMatchingLHSWildcardRHSBothWildcard(rhs);
        } else {
            std::cout << "hitting getMatchingLHSWildcardRHSNoWildcard ie pattern(_, \"v\") for " << lhs << " = " << rhs << "\n";
            return getMatchingLHSWildcardRHSNoWildcard(rhs);
        }
    } else {
        if (pattern == "_") {
            std::cout << "hitting getMatchingLHS ie pattern(\"a\", _) for " << lhs << " = " << rhs << "\n";
            return getMatchingLHS(lhs);
        } else if (leftWildcard && rightWildcard) {
            std::cout << "hitting getMatchingRHSBothWildcard ie pattern(\"a\", _\"v\"_) for " << lhs << " = " << rhs << "\n";
            return getMatchingRHSBothWildcard(lhs, rhs);
        } else {
            std::cout << "hitting getMatchingExact ie pattern(\"a\", \"v\") for " << lhs << " = " << rhs << "\n";
            return getMatchingExact(lhs, rhs);
        }
    }
}

std::unique_ptr<Expression> PatternStorage::buildSubtree(std::string rhs) {
    std::stringstream ss;
    std::deque<Token> tokens;
    Tokenizer tk;
    Parser pr;

    ss << rhs;
    tokens = tk.tokenize(ss);
    std::unique_ptr<Expression> root = std::move(pr.parseExpression(tokens));
    return root;
}

std::vector<StmtNum> PatternStorage::getMatchingExact(std::string lhs, std::string rhs) {
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<StmtNum> empty;
        return empty;
    }
    std::unique_ptr<Expression> expected = buildSubtree(rhs);
    std::vector<StmtNum> res;
    for (const auto& p : lhs_stmtNum_rhsPostfix.at(lhs)) {
        Expression* actual = p.second.get();
        if (isSameTree(expected.get(), actual)) {
            res.push_back(p.first);
        }
    }
    return res;
}

std::vector<StmtNum> PatternStorage::getMatchingRHSBothWildcard(std::string lhs, std::string rhs) {
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<StmtNum> empty;
        return empty;
    }

    std::unique_ptr<Expression> expected = buildSubtree(rhs);
    std::vector<StmtNum> res;
    for (const auto& p : lhs_stmtNum_rhsPostfix.at(lhs)) {
        Expression* actual = p.second.get();
        if (isSubTree(expected.get(), actual)) {
            res.push_back(p.first);
        }
    }
    return res;
}


std::vector<StmtNum> PatternStorage::getMatchingLHS(std::string lhs) {
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<StmtNum> empty;
        return empty;
    }

    std::vector<StmtNum> res;
    for (const auto& p : lhs_stmtNum_rhsPostfix.at(lhs)) {
        res.push_back(p.first);
    }
    return res;
}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSNoWildcard(std::string rhs) {
    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    std::vector<StmtNum> res;

    for (auto const& i : lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            Expression* actual = p.second.get();
            if (isSameTree(expected.get(), actual)) {
                res.push_back(p.first);
            }
        }
    }
    return res;
}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSBothWildcard(std::string rhs) {
    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    std::vector<StmtNum> res;

    for (auto const& i : lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            Expression* actual = p.second.get();
            if (isSubTree(expected.get(), actual)) {
                res.push_back(p.first);
            }
        }
    }
    return res;
}






//std::string getPostfixString(std::string rhs) {
//    std::stringstream ss;
//    std::deque<Token> tokens;
//    Tokenizer tk; Parser pr;
//
//    ss << "dontCare = " + rhs;
//    tokens = tk.tokenize(ss);
//    std::unique_ptr<Expression> root = pr.parseExpression(tokens);
////    unique_ptr<MathExpression> derived(static_cast<MathExpression*>(root.release()));
//
//    std::string res = "";
//    std::deque<std::unique_ptr<Expression>> queue;
//    queue.push_back(std::move(root));
//
//    while (!queue.empty()) {
//        std::unique_ptr<Expression> node = std::move(queue.front());
//        queue.pop_front();
//
//        if (auto i = dynamic_cast<MathExpression*>(node.get())) {
//            queue.push_back(std::move(i -> lhs));
//            queue.push_back(std::move(i -> rhs));
//        }
//        else if (auto i = dynamic_cast<Constant*>(node.get())) {
//            res += std::to_string(i -> value);
//        }
//        else { // Variable
//            res += std::to_string(dynamic_cast<Constant *>(node.get())->value);
//        }
//    }
//    return res;
//}

//std::vector<std::pair<std::string, std::vector<std::string>>> PatternStorage::getAll() {
//    std::vector<std::pair<std::string, std::vector<std::string>>> res;
//    for (auto keyValuePair: lhs_stmtNum_rhsPostfix) {
//        std::string lhs = keyValuePair.first;
//        std::unordered_set<std::string> postfixStrings = keyValuePair.second;
//        for (std::string s: postfixStrings) {
//            res.push_back(std::pair<std::string, std::string>({lhs, s}));
//        }
//    }
//    std::sort(res.begin(), res.end());
//    return res;
//}