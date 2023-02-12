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
     * Structure of table example below.
     * First vector of range_nestedContainerStmts is range
     * Second vector of range_nestedContainerStmts contains the directly nested StmtNums
     *  stmt | range_nestedContainerStmts
     *  11 | [12, 30] ie range of this container is from lines 12 rto 30. There is an if/while at line 15 and line 19
     *  15 | [16, 22]
     *  19 | [20, 26]
     *  22 | [23, 26]
     *
     *
     *  stmt | stmtNum_containerStmtNum
     *  11 | [["if", 15], ["while", 19]]
     *  15 | []
     *  19 | [["while", 22]]
     *  22 | []
     */
    std::unordered_map<StmtNum, std::vector<std::vector<StmtNum>>> stmtNum_range;
    std::unordered_map<StmtNum, std::vector<std::pair<std::string, StmtNum>>> stmtNum_directContainers;
};
