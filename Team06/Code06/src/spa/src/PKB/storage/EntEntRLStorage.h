#pragma once
#include<vector>
#include<utility>
#include<string>

typedef string Ent;

class EntEntRLStorage {
public:
    virtual void write(Ent ent1, Ent ent2);
    virtual bool exists(Ent ent1, Ent ent2);
    virtual std::vector<Ent> getRightWildcard(Ent leftEnt);
    virtual std::vector<Ent> getLeftWildcard(Ent rightEnt);
};