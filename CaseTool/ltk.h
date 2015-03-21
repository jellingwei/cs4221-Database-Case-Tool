#pragma once

#include "attributeset.h"
#include "functionaldependency.h"

namespace ltk {
	using std::set;

	set<Relation> constructPreparatorySchema(AttributeSet, set<FunctionalDependency>);
	AttributeSet superfluousAttributeDetection(set<AttributeSet>, AttributeSet);
	set<Relation> deletionNormalization(AttributeSet, set<FunctionalDependency>);
}