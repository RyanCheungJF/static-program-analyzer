#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>

typedef std::string Stmt;
typedef int StmtNum;

class ContainerStorage {
public:
    virtual void writeContainerBlock(StmtNum num, StmtNum start, StmtNum end, std::vector<StmtNum> directly_nested_containerStmt);
    
private:
    /*
     * Structure of table example below
     *  stmt | range_nestedContainerStmts
     *  11 | [[12, 30], [15, 19]]
     *  15 | [[16, 22], []]
     *  19 | [[20, 26], [22]]
     *  22 | [[23, 26], []]
     */
    std::unordered_map<StmtNum, std::vector<std::vector<StmtNum>>> stmtNum_range_nestedContainerStmts;
    std::unordered_map<StmtNum, std::vector<std::pair<std::string, StmtNum>>> stmtNum_containerStmtNum;
};
