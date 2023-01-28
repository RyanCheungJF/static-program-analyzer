#include "Synonym.h"

Synonym::Synonym() : type(""), name("") {}

Synonym::Synonym(const Synonym& s)
{
	type = s.type;
	name = s.name;
}

Synonym::Synonym(string t, string n)
{
	type = t;
	name = n;
}
