#pragma once

#include "attributeset.h"
#include "functionaldependency.h"

namespace ltk {
	using std::set;

	set<AttributeSet> constructPreparatorySchema(AttributeSet, set<FunctionalDependency>);
	AttributeSet superfluousAttributeDetection(set<AttributeSet>, AttributeSet);
	set<AttributeSet> deletionNormalization(AttributeSet, set<FunctionalDependency>);
}