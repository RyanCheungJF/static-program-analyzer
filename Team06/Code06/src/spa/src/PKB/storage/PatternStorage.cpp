#include "PatternStorage.h"
#include <string>
#include <unordered_map>

/* HELPER FUNCTIONS */
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


std::vector<std::vector<std::string>> PatternStorage::getLHSAndStmtNum() {
    std::vector<std::vector<std::string>> res;

    for (auto const& i : lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            Expression* actual = p.second.get();
            std::vector<std::string> curr;
            curr.push_back(i.first);
            curr.push_back(std::to_string(p.first));
            res.push_back(curr);
        }
    }
    return res;
}


std::vector<std::vector<std::string>> PatternStorage::getLHSAndStmtNumRHSNoWildcard(std::string rhs) {
    std::vector<std::vector<std::string>> res;
    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    for (auto const& i : lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            Expression* actual = p.second.get();
            std::vector<std::string> curr;
            if (isSameTree(expected.get(), actual)) {
                curr.push_back(i.first);
                curr.push_back(std::to_string(p.first));
                res.push_back(curr);
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> PatternStorage::getLHSAndStmtNumRHSBothWildcard(std::string rhs) {
    std::vector<std::vector<std::string>> res;
    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    for (auto const& i : lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            Expression* actual = p.second.get();
            std::vector<std::string> curr;
            if (isSubTree(expected.get(), actual)) {
                curr.push_back(i.first);
                curr.push_back(std::to_string(p.first));
                res.push_back(curr);
            }
        }
    }
    return res;

}


std::vector<std::vector<std::string>> PatternStorage::getMatchingExact(std::string lhs, std::string rhs) {
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<std::vector<std::string>> empty;
        return empty;
    }

    std::unique_ptr<Expression> expected = buildSubtree(rhs);
    std::vector<std::vector<std::string>> res;
    for (const auto& p : lhs_stmtNum_rhsPostfix.at(lhs)) {
        Expression* actual = p.second.get();
        if (isSameTree(expected.get(), actual)) {
            res.push_back({ lhs, std::to_string(p.first) });
        }
    }
    return res;
}

std::vector<std::vector<std::string>> PatternStorage::getMatchingRHSBothWildcard(std::string lhs, std::string rhs) {
    
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<std::vector<std::string>> empty;
        return empty;
    }

    std::unique_ptr<Expression> expected = buildSubtree(rhs);
    std::vector<std::vector<std::string>> res;
    for (const auto& p : lhs_stmtNum_rhsPostfix.at(lhs)) {
        Expression* actual = p.second.get();
        if (isSubTree(expected.get(), actual)) {
            res.push_back({ lhs, std::to_string(p.first) });
        }
    }
    return res;
}


std::vector<std::vector<std::string>> PatternStorage::getMatchingLHS(std::string lhs) {
    
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<std::vector<std::string>> empty;
        return empty;
    }

    std::vector<std::vector<std::string>> res;
    for (const auto& p : lhs_stmtNum_rhsPostfix.at(lhs)) {
        res.push_back({ lhs, std::to_string(p.first) });
    }
    return res;
}

std::vector<std::vector<std::string>> PatternStorage::getMatchingLHSWildcardRHSNoWildcard(std::string rhs) {
    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    std::vector<std::vector<std::string>> res;

    for (auto const& i : lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            Expression* actual = p.second.get();
            std::vector<std::string> curr;
            if (isSameTree(expected.get(), actual)) {
                curr.push_back(i.first);
                curr.push_back(std::to_string(p.first));
                res.push_back(curr);
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> PatternStorage::getMatchingLHSWildcardRHSBothWildcard(std::string rhs) {
    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    std::vector<std::vector<std::string>> res;

    for (auto const& i : lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            Expression* actual = p.second.get();
            std::vector<std::string> curr;
            if (isSubTree(expected.get(), actual)) {
                curr.push_back(i.first);
                curr.push_back(std::to_string(p.first));
                res.push_back(curr);
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


//std::string inorderTraversal(Expression* node, std::string current) {
//
//    // if they are both constants, check they have the same value
//    if (auto leaf = dynamic_cast<Constant*>(node)) {
//        int actualValue = leaf->value;
//        std::string s = std::to_string(actualValue);
//        return s;
//    }
//    else if (auto leaf = dynamic_cast<Variable*> (node)) {
//        std::string s = leaf->name;
//        return s;
//    }
//    else if (auto leaf = dynamic_cast<MathExpression*>(node)) {
//        std::string temp = current;
//        std::string leftString = inorderTraversal(leaf->lhs.get(), current);
//        std::string rightString = inorderTraversal(leaf->rhs.get(), current);
//        return leftString + temp + rightString;
//    }
//    return current;
//
//}