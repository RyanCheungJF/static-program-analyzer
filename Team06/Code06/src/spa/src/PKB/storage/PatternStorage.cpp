#include "PatternStorage.h"
#include <string>
#include <unordered_map>

void PatternStorage::writePattern(std::string lhs, StmtNum num, std::string stmtNum_rhspostfix) {
    lhs_stmtNum_rhsPostfix[lhs].insert(std::make_pair(num, stmtNum_rhspostfix));
    return;
}

std::string PatternStorage::buildSubtree(std::string rhs) {
    std::stringstream ss;
    std::deque<Token> tokens;
    Tokenizer tk;
    Parser pr;

    ss << rhs;
    tokens = tk.tokenize(ss);
    std::unique_ptr<Expression> root = std::move(pr.parseExpression(tokens));

    std::string res;
    std::deque<std::unique_ptr<Expression>> queue;
    queue.push_back(std::move(root));

    while (!queue.empty()) {
        std::unique_ptr<Expression> node = std::move(queue.front());
        queue.pop_front();

        if (auto i = dynamic_cast<MathExpression*>(node.get())) {
            queue.push_back(std::move(i -> lhs));
            queue.push_back(std::move(i -> rhs));
            res += i->mathOperator;
        }
        else if (auto i = dynamic_cast<Constant*>(node.get())) {
            res += std::to_string(i -> value);
        }
        else if (auto i = dynamic_cast<Variable*>(node.get())) {// Variable
            res += i->name;
        }
    }
    return res;

}

std::vector<StmtNum> PatternStorage::getMatchingExact(std::string lhs, std::string rhs) {
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        std::vector<StmtNum> empty;
        return empty;
    }


}

std::vector<StmtNum> PatternStorage::getMatchingRHSLeftWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSRightWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSBothWildcard(std::string lhs, std::string rhs) {}


std::vector<StmtNum> PatternStorage::getMatchingLHS(std::string lhs) {
}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSNoWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSLeftWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSRightWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSBothWildcard(std::string rhs) {}


std::vector<std::pair<std::string, std::string>> PatternStorage::getAllPostfixes() {

    std::vector<std::pair<std::string, std::string>> res;
    for (auto keyValuePair: lhs_rhsPostfix) {
        std::string lhs = keyValuePair.first;
        std::unordered_set<std::string> postfixStrings = keyValuePair.second;

        for (std::string s: postfixStrings) {
            res.push_back(std::pair<std::string, std::string>({lhs, s}));
        }
    }

    std::sort(res.begin(), res.end());
    return res;
}
