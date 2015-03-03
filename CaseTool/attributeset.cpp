#include <algorithm>
#include <set>
#include <iterator>
#include <QDebug> 

#include "attributeset.h"


AttributeSet::AttributeSet(set<int> attr) {
	attributes = attr;
}

bool AttributeSet::isFdApplicable(FunctionalDependency fd) {
	set<int> leftSide = fd.getLhs();

	if (leftSide.size() > this->attributes.size()) {
		return false;
	}

	return containsAttributes(leftSide);
}

bool AttributeSet::containsAttributes(set<int> attr) {
	std::vector<int> intersect;
	set_intersection(this->attributes.begin(), this->attributes.end(), 
					 attr.begin(), attr.end(), 
					 std::inserter(intersect, intersect.begin()));
	
	return intersect.size() == attr.size();
}

AttributeSet AttributeSet::getAttributeClosure(set<FunctionalDependency> fdSet) {
	bool isResultSetChanged = true;
	AttributeSet resultSet(*this); // init the closure with the starting attr

	while (isResultSetChanged) {
		isResultSetChanged = false;
		for (auto iter = fdSet.begin(); iter != fdSet.end(); ++iter) {
			
			FunctionalDependency fd = *iter;

			if (!resultSet.isFdApplicable(fd)) {
				continue;
			}
			
			// update resultSet with the RHS of the functional dependencies
			set<int> attributes = fd.getRhs();
			if (resultSet.containsAttributes(attributes)) {
				// skip if all attributes in rhs is already in the closure
				continue;
			}

			AttributeSet newAttributes(attributes);
			AttributeSet newResultSet = resultSet.addAttributesToSet(newAttributes);

			resultSet = newResultSet;
			isResultSetChanged = true;
		}
	}

	return resultSet;
}

AttributeSet AttributeSet::addAttributesToSet(AttributeSet attributesToAdd) {
	set<int> attributes = this->attributes;
	set<int> newAttributes = attributesToAdd.getAttributes();

	attributes.insert(newAttributes.begin(), newAttributes.end());
	return AttributeSet(attributes);
}

set<AttributeSet> AttributeSet::constructRelations(set<FunctionalDependency> fdSet) {
	set<AttributeSet> constructedRelations;

	for (auto fdSetItr = fdSet.begin(); fdSetItr != fdSet.end(); ++fdSetItr) {
		set<int> relationAttributes;
		FunctionalDependency fd = *fdSetItr;

		set<int> fdLhs = fd.getLhs();
		for (auto fdItr = fdLhs.begin(); fdItr != fdLhs.end(); ++fdItr) {
			relationAttributes.insert(*fdItr);
		}

		set<int> fdRhs = fd.getRhs();
		for (auto fdItr = fdRhs.begin(); fdItr != fdRhs.end(); ++fdItr) {
			relationAttributes.insert(*fdItr);
		}

		AttributeSet newRelation(relationAttributes);
		constructedRelations.insert(newRelation);
	}
	return constructedRelations;
}

set<int> AttributeSet::getAttributes() {
	return attributes;
}

int AttributeSet::size() {
	return attributes.size();
}
