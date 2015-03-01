#pragma once

#include <set>
#include "functionaldependency.h"

using std::set;

class FunctionalDependency;

class AttributeSet {
public:
	AttributeSet(set<int>);
	AttributeSet getAttributeClosure(set<FunctionalDependency>);
	bool isFdApplicable(FunctionalDependency);
	bool containsAttributes(set<int>);

	AttributeSet addAttributesToSet(AttributeSet);
	set<int> getAttributes();
	int size();

private:
	set<int> attributes;

};