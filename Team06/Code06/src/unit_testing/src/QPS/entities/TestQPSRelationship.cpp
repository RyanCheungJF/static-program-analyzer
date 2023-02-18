#include "catch.hpp"
#include <iostream>
#include "qps/entities/Relationship.h"
#include "qps/entities/FollowsRelationship.h"
#include "qps/entities/FollowsTRelationship.h"
#include "qps/entities/ParentRelationship.h"
#include "qps/entities/ParentTRelationship.h"
#include "qps/entities/UsesRelationship.h"
#include "qps/entities/ModifiesRelationship.h"

TEST_CASE("makeRelationship / empty relationship type string / throws error") {
    AppConstants CONSTANTS;
	string typeInput = "";
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("b", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / unknown relationship type string / throws error") {
    AppConstants CONSTANTS;
	string typeInput = "unknown type";
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("b", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / follows relationship, with correct params / returns relationship object with correct fields and types") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	Parameter p3("_", CONSTANTS.WILDCARD);
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
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2(CONSTANTS.FIXED_STRING, CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / follows relationship, with wrong first param / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1(CONSTANTS.FIXED_STRING, CONSTANTS.FIXED_STRING);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / follows relationship, more than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("b", CONSTANTS.SYNONYM);
	Parameter p3("c", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / follows relationship, less than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}


TEST_CASE("makeRelationship / FollowsT relationship / return relationship object") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWST;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	Parameter p3("_", CONSTANTS.WILDCARD);
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
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWST;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("entity_ref", CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / FollowsT, wrong 1st param / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWST;
	Parameter p1("entity_ref", CONSTANTS.FIXED_STRING);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / FollowsT relationship, more than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWST;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("b", CONSTANTS.SYNONYM);
	Parameter p3("c", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / FollowsT relationship, less than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWST;
	Parameter p1("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Parent relationship / return relationship object") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	Parameter p3("_", CONSTANTS.WILDCARD);
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
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("entity_ref", CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Parent, wrong 1st param / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENT;
	Parameter p1("entity_ref", CONSTANTS.FIXED_STRING);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Parent relationship, more than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("b", CONSTANTS.SYNONYM);
	Parameter p3("c", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Parent relationship, less than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / ParentT relationship / return relationship object") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENTT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	Parameter p3("_", CONSTANTS.WILDCARD);
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
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENTT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("entity_ref", CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / ParentT, wrong 1st param / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENTT;
	Parameter p1("entity_ref", CONSTANTS.FIXED_STRING);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / ParentT relationship, more than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENTT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("b", CONSTANTS.SYNONYM);
	Parameter p3("c", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / ParentT relationship, less than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENTT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Uses relationship / return relationship object") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.USES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("_", CONSTANTS.WILDCARD);
	Parameter p3("ent_ref", CONSTANTS.FIXED_STRING);
	Parameter p4("1", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };
	vector<Parameter> inputParameters2{ p4, p3 };
	vector<Parameter> inputParameters3{ p2, p3 };
	RelationshipType expectedType = RelationshipType::USES;

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	UsesRelationship expected(inputParameters);
	CHECK(expected == *output);
	shared_ptr<Relationship> output2 = Relationship::makeRelationship(typeInput, inputParameters2);
	UsesRelationship expected2(inputParameters2);
	CHECK(expected2 == *output2);
	shared_ptr<Relationship> output3 = Relationship::makeRelationship(typeInput, inputParameters3);
	UsesRelationship expected3(inputParameters3);
	CHECK(expected3 == *output3);
}

TEST_CASE("makeRelationship / Uses, wrong 1st param / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.USES;
	Parameter p1("ent_ref", CONSTANTS.VARIABLE);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Uses, wrong 2nd param / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.USES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("stmt_ref", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Uses relationship, more than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.USES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("b", CONSTANTS.SYNONYM);
	Parameter p3("c", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Uses relationship, less than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.USES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Modifies relationship / return relationship object") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.MODIFIES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("_", CONSTANTS.WILDCARD);
	Parameter p3("ent_ref", CONSTANTS.FIXED_STRING);
	Parameter p4("1", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };
	vector<Parameter> inputParameters2{ p4, p3 };
	vector<Parameter> inputParameters3{ p2, p3 };
	RelationshipType expectedType = RelationshipType::USES;

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	ModifiesRelationship expected(inputParameters);
	CHECK(expected == *output);
	shared_ptr<Relationship> output2 = Relationship::makeRelationship(typeInput, inputParameters2);
	ModifiesRelationship expected2(inputParameters2);
	CHECK(expected2 == *output2);
	shared_ptr<Relationship> output3 = Relationship::makeRelationship(typeInput, inputParameters3);
	ModifiesRelationship expected3(inputParameters3);
	CHECK(expected3 == *output3);
}

TEST_CASE("makeRelationship / Modifies, wrong 1st param / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.MODIFIES;
	Parameter p1("ent_ref", CONSTANTS.VARIABLE);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Modifies, wrong 2nd param / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.MODIFIES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("stmt_ref", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };
	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Modifies relationship, more than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.MODIFIES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("b", CONSTANTS.SYNONYM);
	Parameter p3("c", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2, p3 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("makeRelationship / Modifies relationship, less than 2 parameter / throws error") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.MODIFIES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1 };

	REQUIRE_THROWS(Relationship::makeRelationship(typeInput, inputParameters));
}

TEST_CASE("FollowsRelationship.validateParams / invalid 1st param / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}

TEST_CASE("FollowsRelationship.validateParams / invalid 2nd params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("1", CONSTANTS.FIXED_INT);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(1).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}

TEST_CASE("FollowsRelationship.validateParams / valid params / return true") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::ASSIGN);
	CHECK(output->validateParams());
}

TEST_CASE("FollowsTRelationship.validateParams / invalid 1st params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWST;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}

TEST_CASE("FollowsTRelationship.validateParams / invalid 2nd params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWST;
	Parameter p1("1", CONSTANTS.FIXED_INT);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(1).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}


TEST_CASE("FollowsTRelationship.validateParams / valid params / return true") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWST;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::STMT);
	CHECK(output->validateParams());
}

TEST_CASE("ParentRelationship.validateParams / invalid 1st params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}

TEST_CASE("ParentRelationship.validateParams / invalid 2nd params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENT;
	Parameter p1("1", CONSTANTS.FIXED_INT);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(1).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}

TEST_CASE("ParentRelationship.validateParams / valid params / return true") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENT;
	Parameter p1("1", CONSTANTS.FIXED_INT);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(1).updateSynonymType(ParameterType::READ);
	CHECK(output->validateParams());
}

TEST_CASE("ParentTRelationship.validateParams / invalid 1st params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENTT;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("1", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}

TEST_CASE("ParentTRelationship.validateParams / invalid 2nd params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENTT;
	Parameter p1("1", CONSTANTS.FIXED_INT);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(1).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}

TEST_CASE("ParentTRelationship.validateParams / valid params / return true") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.PARENTT;
	Parameter p1("1", CONSTANTS.FIXED_INT);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(1).updateSynonymType(ParameterType::WHILE);
	CHECK(output->validateParams());
}

TEST_CASE("ModifiesRelationship.validateParams / invalid 1st params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.MODIFIES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("a", CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}

TEST_CASE("ModifiesRelationship.validateParams / invalid 2nd params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.MODIFIES;
	Parameter p1("1", CONSTANTS.FIXED_INT);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(1).updateSynonymType(ParameterType::PROCEDURE);
	CHECK(!output->validateParams());
}

TEST_CASE("ModifiesRelationship.validateParams / 1st param is procedure / return true") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.MODIFIES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("a", CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::PROCEDURE);
	CHECK(output->validateParams());
}

TEST_CASE("ModifiesRelationship.validateParams / 1st param is stmt ref / return true") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.MODIFIES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("a", CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::CALL);
	CHECK(output->validateParams());
}

TEST_CASE("UsesRelationship.validateParams / invalid 1st params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.USES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("a", CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::VARIABLE);
	CHECK(!output->validateParams());
}

TEST_CASE("UsesRelationship.validateParams / invalid 2nd params / return false") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.USES;
	Parameter p1("1", CONSTANTS.FIXED_INT);
	Parameter p2("a", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(1).updateSynonymType(ParameterType::PROCEDURE);
	CHECK(!output->validateParams());
}

TEST_CASE("UsesRelationship.validateParams / 1st param is procedure / return true") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.USES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("a", CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::PROCEDURE);
	CHECK(output->validateParams());
}

TEST_CASE("UsesRelationship.validateParams / 1st param is stmt ref / return true") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.USES;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("a", CONSTANTS.FIXED_STRING);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	output->params.at(0).updateSynonymType(ParameterType::CALL);
	CHECK(output->validateParams());
}

TEST_CASE("getAllUncheckedSynonyms / contains no synonyms / return empty list ") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("1", CONSTANTS.FIXED_INT);
	Parameter p2("2", CONSTANTS.FIXED_INT);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	vector<Parameter*> synonyms = output->getAllUncheckedSynonyms();
	CHECK(synonyms.empty());
}

TEST_CASE("getAllUncheckedSynonyms / contains one synonyms / return list with pointer to synonym") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("2", CONSTANTS.FIXED_INT);
	Parameter p1After("a", CONSTANTS.STMT);
	vector<Parameter> inputParameters{ p1, p2 };
	vector<Parameter> afterParameters{ p1After, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	shared_ptr<Relationship> expected = Relationship::makeRelationship(typeInput, afterParameters);
	vector<Parameter*> synonyms = output->getAllUncheckedSynonyms();
	REQUIRE(synonyms.size() == 1);
	Parameter* pp = synonyms.at(0);
	pp->updateSynonymType(ParameterType::STMT);
	REQUIRE((*output) == (*expected));
}

TEST_CASE("getAllUncheckedSynonyms / contains more than one synonym / return list with pointer to the synonyms") {
    AppConstants CONSTANTS;
	string typeInput = CONSTANTS.FOLLOWS;
	Parameter p1("a", CONSTANTS.SYNONYM);
	Parameter p2("b", CONSTANTS.SYNONYM);
	vector<Parameter> inputParameters{ p1, p2 };

	shared_ptr<Relationship> output = Relationship::makeRelationship(typeInput, inputParameters);
	vector<Parameter*> synonyms = output->getAllUncheckedSynonyms();
	REQUIRE(synonyms.size() == 2);
}
