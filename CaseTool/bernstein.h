#pragma once
#include <unordered_map>

#include "attributeset.h"
#include "functionaldependency.h"

namespace bernstein {
	using std::unordered_map;
	using std::set;

	set<FunctionalDependency> decomposeFd(set<FunctionalDependency> );

	set<FunctionalDependency> removeRedundantAttributes(set<FunctionalDependency>);
	set<FunctionalDependency> obtainMinimalCover(set<FunctionalDependency>);

	unordered_map<AttributeSet, set<FunctionalDependency> > partitionFd(set<FunctionalDependency>);
	unordered_map<AttributeSet, set<FunctionalDependency> > mergeEquivalentKeys(set<FunctionalDependency>);

	unordered_map<AttributeSet, set<FunctionalDependency> > eliminateTransitiveDependencies(set<FunctionalDependency>);
	set<AttributeSet> constructRelations(unordered_map<AttributeSet, set<FunctionalDependency> >);
}
