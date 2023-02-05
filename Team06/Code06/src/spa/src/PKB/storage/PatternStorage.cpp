#include "PatternStorage.h"
#include <string>
#include <unordered_map>

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
        if (isSameTree(p.second, expected)) {
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

    std::vector<StmtNum> res;
    std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction> set = std::move(lhs_stmtNum_rhsPostfix.at(lhs));

    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    for (const auto& p : set) {
        if (isSubTree(p.second, expected)) {
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
    std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction> set = std::move(lhs_stmtNum_rhsPostfix.at(lhs));
    for (const auto& p : set) {
        res.push_back(p.first);
    }
    return res;
}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSNoWildcard(std::string rhs) {
    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    std::vector<StmtNum> res;

    for (auto i: lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            if (isSameTree(p.second, expected)) {
                res.push_back(p.first);
            }
        }
    }
    return res;
}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSBothWildcard(std::string rhs) {
    std::unique_ptr<Expression> expected = std::move(buildSubtree(rhs));
    std::vector<StmtNum> res;

    for (auto i: lhs_stmtNum_rhsPostfix) {
        for (const auto& p : i.second) {
            if (isSubTree(p.second, expected)) {
                res.push_back(p.first);
            }
        }
    }
    return res;
}

/*
std::vector<std::pair<std::string, std::string>> PatternStorage::getAllPostfixes() {

    std::vector<std::pair<std::string, std::string>> res;
    for (auto keyValuePair: lhs_stmtNum_rhsPostfix) {
        std::string lhs = keyValuePair.first;
        std::unordered_set<std::string> postfixStrings = keyValuePair.second;

        for (std::string s: postfixStrings) {
            res.push_back(std::pair<std::string, std::string>({lhs, s}));
        }
    }

    std::sort(res.begin(), res.end());
    return res;
}
*/
