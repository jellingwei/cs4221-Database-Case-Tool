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

	if (resultSet.getAttributes().size() == 0) {
		return resultSet;
	}

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

set<int> AttributeSet::getAttributes() const {
	return attributes;
}

int AttributeSet::size() {
	return attributes.size();
}
