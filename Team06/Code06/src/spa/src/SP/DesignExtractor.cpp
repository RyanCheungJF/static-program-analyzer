#include "DesignExtractor.h"

void DesignExtractor::extractRelationships(Program* program) {
	FollowsExtractor followsExtractor;
	// starts from root to extract out relationships
	program->accept(&followsExtractor);
};
