#include "PatternStorage.h"


void PatternStorage::writePatternNode(Node &assignNode) {
    std::string rhsVariable = assignNode.left->value;
    rhsVariable_Nodes[rhsVariable].insert(&assignNode);
    return;
}



std::string PatternStorage::buildSubtree(std::string rhs) {
    std::stringstream ss;
    std::deque<Token> tokens;
    Tokenizer tk;
    Parser pr;

    ss << "x / y * z";
    tokens = tk.tokenize(ss);
    std::unique_ptr<Expression> root = std::move(pr.parseExpression(tokens));

//    unique_ptr<MathExpression> derived(static_cast<MathExpression*>(root.release()));

    std::string res = "";
    std::deque<std::unique_ptr<Expression>> queue;
    queue.push_back(std::move(root));

    while (!queue.empty()) {
        std::unique_ptr<Expression> node = std::move(queue.front());
        queue.pop_front();

        if (auto i = dynamic_cast<MathExpression*>(node.get())) {
            queue.push_back(std::move(i -> lhs));
            queue.push_back(std::move(i -> rhs));
            res += i -> mathOperator;
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


std::vector<StmtNum> PatternStorage::getAllAssignStatements() {
    std::unordered_set<StmtNum> set;
    for (auto keyValuePair: rhsVariable_Nodes) {
        std::string rhs = keyValuePair.first;
        std::unordered_set<Node*> value = keyValuePair.second;

        for (Node* ptr: value) {
            set.insert(ptr->line);
        }
    }

    std::vector<StmtNum> res;
    for (StmtNum num: set) {
        res.push_back(num);
    }
    std::sort(res.begin(), res.end());
    return res;
}

std::vector<StmtNum> PatternStorage::getMatchingExact(std::string lhs, std::string rhs) {
}

std::vector<StmtNum> PatternStorage::getMatchingRHSLeftWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSRightWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSBothWildcard(std::string lhs, std::string rhs) {}


std::vector<StmtNum> PatternStorage::getMatchingLHS(std::string lhs) {
    // if lhs does not exist
    if (rhsVariable_Nodes.find(lhs) == rhsVariable_Nodes.end()) {
        std::vector<StmtNum> empty;
        return empty;
    }

    std::unordered_set<Node*> set = rhsVariable_Nodes[lhs];
    std::vector<StmtNum> res;
    for (Node* ptr: set) {
        res.push_back(ptr->line);
    }
    std::sort(res.begin(), res.end());
    return res;
}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSNoWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSLeftWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSRightWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSBothWildcard(std::string rhs) {}