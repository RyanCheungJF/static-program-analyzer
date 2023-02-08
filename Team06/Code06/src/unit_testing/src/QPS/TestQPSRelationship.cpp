#include "catch.hpp"
#include <iostream>
#include "qps/entities/Relationship.h"

TEST_CASE("makeRelationship / follows relationship, with correct params / returns relationship object with correct fields and types") {
	string typeInput = "Follows";
	Parameter p1("a", "synonym");
	Parameter p2("b", "synonym");
	vector<Parameter> inputParameters{p1, p2};
	RelationshipType expectedType = RelationshipType::FOLLOWS;
	vector<Parameter> expectedParameters = inputParameters;

	Relationship output = Relationship::makeRelationship(typeInput, inputParameters);
	CHECK(expectedType == output.type);
	//REQUIRE(expectedParameters == inputParameters);
}

TEST_CASE("makeRelationship / follows relationship, with wrong params / throws error") {
	string typeInput = "Follows";
	Parameter p1("a", "synonym");
	Parameter p2("\"fixed_string\"", "synonym");
	vector<Parameter> inputParameters{ p1, p2 };
	RelationshipType expectedType = RelationshipType::FOLLOWS;

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
	//REQUIRE(expectedParameters == inputParameters);
}