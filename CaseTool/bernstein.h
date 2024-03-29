#pragma once
#include <unordered_map>

#include "attributeset.h"
#include "functionaldependency.h"
#include "normaltest.h"

namespace bernstein {
	using std::unordered_map;
	using std::set;
	using std::vector;

	vector<std::pair<int, FunctionalDependency> > getMessageForStep1();
	vector<FunctionalDependency > getMessageForRemovingTransitiveDependencies();

	set<FunctionalDependency> decomposeFd(set<FunctionalDependency> );

	set<FunctionalDependency> removeTrivialDependencies(set<FunctionalDependency> fdSet);
	set<FunctionalDependency> removeRedundantAttributes(set<FunctionalDependency>);
	set<FunctionalDependency> eliminateTransitiveDependencies(set<FunctionalDependency>);
	set<FunctionalDependency> obtainMinimalCover(set<FunctionalDependency>);

	unordered_map<AttributeSet, set<FunctionalDependency> > partitionFd(set<FunctionalDependency>);
	unordered_map<AttributeSet, set<FunctionalDependency> > mergeEquivalentKeys(unordered_map<AttributeSet, set<FunctionalDependency> >, set<FunctionalDependency>);
	unordered_map<AttributeSet, set<FunctionalDependency> > addFdInJBackToCorrespondingGroup(unordered_map<AttributeSet, set<FunctionalDependency> > partitions);
	unordered_map<AttributeSet, set<FunctionalDependency> > eliminateTransitiveDependencies(unordered_map<AttributeSet, set<FunctionalDependency> >, set<FunctionalDependency>);

	set<FunctionalDependency> createSetOfFDFromPartitions(unordered_map<AttributeSet, set<FunctionalDependency> >);
	unordered_map<AttributeSet, set<FunctionalDependency> > eliminateTransitiveDependenciesForPartition(unordered_map<AttributeSet, set<FunctionalDependency> > , set<FunctionalDependency> ); // for step 5
	set<std::pair<AttributeSet, set<AttributeSet> > > constructRelations(unordered_map<AttributeSet, set<FunctionalDependency> >);

	set<AttributeSet> findCandidateKeys(AttributeSet, set<FunctionalDependency> ); 
	std::pair<AttributeSet, set<AttributeSet> >  constructMissingAttrRelation(set<std::pair<AttributeSet, set<AttributeSet> > >, int sizeOfAttrs, AttributeSet key);
}
