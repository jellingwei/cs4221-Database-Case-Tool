#pragma once

#include <unordered_map>

#include "attributeset.h"
#include "functionaldependency.h"

namespace ltk {
	using std::set;
	using std::unordered_map;

	set<Relation> constructPreparatorySchema(set<FunctionalDependency>, int);
	set<AttributeSet> superfluousAttributeDetection(Relation, AttributeSet);
	unordered_map<AttributeSet, set<FunctionalDependency> > createSynthesizedFDs(set<Relation> relationSet);
}