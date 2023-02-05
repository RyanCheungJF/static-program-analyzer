#pragma once

#include "PKB.h"

typedef int StmtNum;

class WritePKB {
public:

    // Sets the pointer to the PKB instance if it is not set yet
    void setInstancePKB(PKB& pkb);

    // Sets Follows relation in PKB
    void setFollows(StmtNum left, StmtNum right);


    // Sets FollowsT relation in PKB
    void setFollowsT(std::vector<std::pair<StmtNum, StmtNum>> followee_follower);


    // Sets Parent relation in PKB
    void setParent(StmtNum parent, StmtNum child);


    // Sets the statement along with the statement lines that they appear in
    void setStatement(Stmt s, std::vector<StmtNum> lines);

    // Sets the statement along with the statement line that it appears in
    void setStatement(Stmt s, StmtNum line);


    // Sets the entity along with the statement line that the entities appears in
    void setEntity(StmtNum line, std::vector<Ent> entities);

    // Sets the entity along with the statement line that it appears in
    void setEntity(Ent e, StmtNum line);


    // Sets the procedure along with the statement lines that are in that procedure appears in
    void setProcedure(Proc p, std::vector<StmtNum> lines);

    // Sets statement line to the procedure that it is in
    void setProcedure(Proc p, StmtNum line);


    // Sets the constant to the statement line that it appears in
    void setConstant(Const c, StmtNum line);

    void setConstant(StmtNum line, std::vector<Const> constants);

    void writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer);

private:
    PKB* pkbInstance = NULL;
};