#include "catch.hpp"
#include <iostream>
#include "qps/entities/Relationship.h"
#include "qps/entities/FollowsRelationship.h"
#include "qps/entities/FollowsTRelationship.h"
#include "qps/entities/ParentRelationship.h"
#include "qps/entities/ParentTRelationship.h"

TEST_CASE("makeRelationship / empty relationship type string / throws error") {
	string typeInput = "";
	Parameter p1("a", "synonym");
	Parameter p2("b", "synonym");
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / unknown relationship type string / throws error") {
	string typeInput = "unknown type";
	Parameter p1("a", "synonym");
	Parameter p2("b", "synonym");
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / follows relationship, with correct params / returns relationship object with correct fields and types") {
	string typeInput = "Follows";
	Parameter p1("a", "synonym");
	Parameter p2("1", "fixed_int");
	Parameter p3("_", "wildcard");
	vector<Parameter> inputParameters{p1, p2};
	vector<Parameter> inputParameters2{p2, p3};
	vector<Parameter> inputParameters3{p3, p1};

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	FollowsRelationship expected(inputParameters);
	CHECK(expected == *output);
	shared_ptr<Relationship> output2 = Relationship::makeRelationship(typeInput, inputParameters2);
	FollowsRelationship expected2(inputParameters2);
	CHECK(expected2 == *output2);
	shared_ptr<Relationship> output3 = Relationship::makeRelationship(typeInput, inputParameters3);
	FollowsRelationship expected3(inputParameters3);
	CHECK(expected3 == *output3);
}

TEST_CASE("makeRelationship / follows relationship, with wrong second param / throws error") {
	string typeInput = "Follows";
	Parameter p1("a", "synonym");
	Parameter p2("fixed_string", "fixed_string");
	vector<Parameter> inputParameters{ p1, p2 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / follows relationship, with wrong first param / throws error") {
	string typeInput = "Follows";
	Parameter p1("fixed_string", "fixed_string");
	Parameter p2("a", "synonym");
	vector<Parameter> inputParameters{ p1, p2 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / follows relationship, more than 2 parameter / throws error") {
	string typeInput = "Follows";
	Parameter p1("a", "synonym");
	Parameter p2("b", "synonym");
	Parameter p3("c", "synonym");
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / follows relationship, less than 2 parameter / throws error") {
	string typeInput = "Follows";
	Parameter p1("a", "synonym");
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}


TEST_CASE("makeRelationship / FollowsT relationship / return relationship object") {
	string typeInput = "Follows*";
	Parameter p1("a", "synonym");
	Parameter p2("1", "fixed_int");
	Parameter p3("_", "wildcard");
	vector<Parameter> inputParameters{ p1, p2 };
	vector<Parameter> inputParameters2{ p2, p3 };
	vector<Parameter> inputParameters3{ p3, p1 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	FollowsTRelationship expected(inputParameters);
	CHECK(expected == *output);
	shared_ptr<Relationship> output2 = Relationship::makeRelationship(typeInput, inputParameters2);
	FollowsTRelationship expected2(inputParameters2);
	CHECK(expected2 == *output2);
	shared_ptr<Relationship> output3 = Relationship::makeRelationship(typeInput, inputParameters3);
	FollowsTRelationship expected3(inputParameters3);
	CHECK(expected3 == *output3);
}

TEST_CASE("makeRelationship / FollowsT, wrong 2nd param / throws error") {
	string typeInput = "Follows*";
	Parameter p1("a", "synonym");
	Parameter p2("entity_ref", "fixed_string");
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / FollowsT, wrong 1st param / throws error") {
	string typeInput = "Follows*";
	Parameter p1("entity_ref", "fixed_string");
	Parameter p2("a", "synonym");
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / FollowsT relationship, more than 2 parameter / throws error") {
	string typeInput = "Follows*";
	Parameter p1("a", "synonym");
	Parameter p2("b", "synonym");
	Parameter p3("c", "synonym");
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / FollowsT relationship, less than 2 parameter / throws error") {
	string typeInput = "Follows*";
	Parameter p1("a", "synonym");
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Parent relationship / return relationship object") {
	string typeInput = "Parent";
	Parameter p1("a", "synonym");
	Parameter p2("1", "fixed_int");
	Parameter p3("_", "wildcard");
	vector<Parameter> inputParameters{ p1, p2 };
	vector<Parameter> inputParameters2{ p2, p3 };
	vector<Parameter> inputParameters3{ p3, p1 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	ParentRelationship expected(inputParameters);
	CHECK(expected == *output);
	shared_ptr<Relationship> output2 = Relationship::makeRelationship(typeInput, inputParameters2);
	ParentRelationship expected2(inputParameters2);
	CHECK(expected2 == *output2);
	shared_ptr<Relationship> output3 = Relationship::makeRelationship(typeInput, inputParameters3);
	ParentRelationship expected3(inputParameters3);
	CHECK(expected3 == *output3);
}

TEST_CASE("makeRelationship / Parent, wrong 2nd param / throws error") {
	string typeInput = "Parent";
	Parameter p1("a", "synonym");
	Parameter p2("entity_ref", "fixed_string");
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Parent, wrong 1st param / throws error") {
	string typeInput = "Parent";
	Parameter p1("entity_ref", "fixed_string");
	Parameter p2("a", "synonym");
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Parent relationship, more than 2 parameter / throws error") {
	string typeInput = "Parent";
	Parameter p1("a", "synonym");
	Parameter p2("b", "synonym");
	Parameter p3("c", "synonym");
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Parent relationship, less than 2 parameter / throws error") {
	string typeInput = "Parent";
	Parameter p1("a", "synonym");
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / ParentT relationship / return relationship object") {
	string typeInput = "Parent*";
	Parameter p1("a", "synonym");
	Parameter p2("1", "fixed_int");
	Parameter p3("_", "wildcard");
	vector<Parameter> inputParameters{ p1, p2 };
	vector<Parameter> inputParameters2{ p2, p3 };
	vector<Parameter> inputParameters3{ p3, p1 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	ParentTRelationship expected(inputParameters);
	CHECK(expected == *output);
	shared_ptr<Relationship> output2 = Relationship::makeRelationship(typeInput, inputParameters2);
	ParentTRelationship expected2(inputParameters2);
	CHECK(expected2 == *output2);
	shared_ptr<Relationship> output3 = Relationship::makeRelationship(typeInput, inputParameters3);
	ParentTRelationship expected3(inputParameters3);
	CHECK(expected3 == *output3);
}

TEST_CASE("makeRelationship / ParentT, wrong 2nd param / throws error") {
	string typeInput = "Parent*";
	Parameter p1("a", "synonym");
	Parameter p2("entity_ref", "fixed_string");
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / ParentT, wrong 1st param / throws error") {
	string typeInput = "Parent*";
	Parameter p1("entity_ref", "fixed_string");
	Parameter p2("a", "synonym");
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / ParentT relationship, more than 2 parameter / throws error") {
	string typeInput = "Parent*";
	Parameter p1("a", "synonym");
	Parameter p2("b", "synonym");
	Parameter p3("c", "synonym");
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / ParentT relationship, less than 2 parameter / throws error") {
	string typeInput = "Parent*";
	Parameter p1("a", "synonym");
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Uses relationship / return relationship object") {
	string typeInput = "Uses";
	Parameter p1("a", "synonym");
	Parameter p2("_", "wildcard");
	Parameter p3("ent_ref", "fixed_string");
	Parameter p4("1", "fixed_int");
	vector<Parameter> inputParameters{ p1, p2 };
	RelationshipType expectedType = RelationshipType::USES;

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	//CHECK(expectedType == output.type);
}

TEST_CASE("makeRelationship / Modifies relationship / return relationship object") {
	string typeInput = "Modifies";
	Parameter p1("a", "synonym");
	Parameter p2("b", "synonym");
	vector<Parameter> inputParameters{ p1, p2 };
	RelationshipType expectedType = RelationshipType::MODIFIES;

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	//CHECK(expectedType == output.type);
}

TEST_CASE("test validate params invalid params") {
	string typeInput = "Follows";
	Parameter p1("a", "synonym");
	Parameter p2("1", "fixed_int");
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::VARIABLE);
	CHECK(output->validateParams() == false);
}

TEST_CASE("test validate params valid params") {
	string typeInput = "Follows";
	Parameter p1("a", "synonym");
	Parameter p2("1", "fixed_int");
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::ASSIGN);
	CHECK(output->validateParams());
}