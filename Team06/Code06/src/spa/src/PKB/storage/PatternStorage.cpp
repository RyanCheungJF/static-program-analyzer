#include "PatternStorage.h"
#include <string>
#include <unordered_map>

bool isSameTree(std::unique_ptr<Expression> expectedExpression, Expression* actual) {
    auto expected = expectedExpression.get();
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

        bool isSameLeftExpression = isSameTree(std::move(expectedExpression->lhs), actualExpression->lhs.get());
        bool isSameRightExpression = isSameTree(std::move(expectedExpression->rhs), actualExpression->rhs.get());
        bool isSameOp = (expectedExpression->mathOperator == actualExpression->mathOperator);
        return (isSameLeftExpression && isSameRightExpression && isSameOp);
    }
    return false;
}

bool isSubTree(std::unique_ptr<Expression> subTreeExpression, std::unique_ptr<Expression> treeExpression) {
    // check if they are the same tree first 
    if (isSameTree(std::move(subTreeExpression), treeExpression.get())) {
        return true;
    }

    auto tree = treeExpression.get();

    if (dynamic_cast<MathExpression*>(tree)) {
        auto t = dynamic_cast<MathExpression*>(tree);
        bool isLeftSubtree = isSubTree(std::move(subTreeExpression), std::move(t->lhs));
        bool isRightSubtree = isSubTree(std::move(subTreeExpression), std::move(t->rhs));
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

std::unique_ptr<Expression> PatternStorage::buildSubtree(std::string rhs) {
    std::stringstream ss;
    std::deque<Token> tokens;
    Tokenizer tk;
    Parser pr;

    ss << rhs;
    tokens = tk.tokenize(ss);
    std::unique_ptr<Expression> root = std::move(pr.parseExpression(tokens));
    return root;

    //    std::string res;
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
    //            res += i->mathOperator;
    //        }
    //        else if (auto i = dynamic_cast<Constant*>(node.get())) {
    //            res += std::to_string(i -> value);
    //        }
    //        else if (auto i = dynamic_cast<Variable*>(node.get())) {// Variable
    //            res += i->name;
    //        }
    //    }
    //    return res;

}

std::vector<StmtNum> PatternStorage::getMatchingExact(std::string lhs, std::string rhs) {
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<StmtNum> empty;
        return empty;
    }

    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    std::vector<StmtNum> res;
    std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction> set = std::move(lhs_stmtNum_rhsPostfix.at(lhs));

    for (const auto& p : set) {
        //        if (isSameTree(p.second, expected)) {
        //            res.push_back(p.first);
        //        }
    }
    return res;
}

std::vector<StmtNum> PatternStorage::getMatchingRHSBothWildcard(std::string lhs, std::string rhs) {
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<StmtNum> empty;
        return empty;
    }

    std::vector<StmtNum> res;
    std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction> set = std::move(lhs_stmtNum_rhsPostfix.at(lhs));

    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    for (const auto& p : set) {
        //        if (isSubTree(p.second, expected)) {
        //            res.push_back(p.first);
        //        }
    }
    return res;
}


std::vector<StmtNum> PatternStorage::getMatchingLHS(std::string lhs) {
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<StmtNum> empty;
        return empty;
    }

    std::vector<StmtNum> res;
    std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction> set = std::move(lhs_stmtNum_rhsPostfix.at(lhs));
    for (const auto& p : set) {
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
            if (isSameTree(std::move(expected), actual)) {
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
            //            if (isSubTree(p.second, expected)) {
            //                res.push_back(p.first);
            //            }
        }
    }
    return res;
}


std::vector<std::pair<std::string, std::string>> PatternStorage::getAllPostfixes() {
    std::vector<std::pair<std::string, std::string>> res;
    /*for (auto keyValuePair: lhs_stmtNum_rhsPostfix) {
        std::string lhs = keyValuePair.first;
        std::unordered_set<std::string> postfixStrings = keyValuePair.second;
        for (std::string s: postfixStrings) {
            res.push_back(std::pair<std::string, std::string>({lhs, s}));
        }
    }
    std::sort(res.begin(), res.end());*/
    return res;
}