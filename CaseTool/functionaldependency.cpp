#include <set>

#include "functionaldependency.h"
#include "attributeset.h"

using namespace std;


FunctionalDependency::FunctionalDependency(set<int> leftSide, set<int> rightSide) {
	lhs = leftSide;
	rhs = rightSide;
}

FunctionalDependency::FunctionalDependency(AttributeSet leftSide, AttributeSet rightSide) {
	lhs = leftSide.getAttributes();
	rhs = rightSide.getAttributes();
}



set<int> FunctionalDependency::getLhs() {
	return lhs;
}

set<int> FunctionalDependency::getRhs() {
	return rhs;
}

AttributeSet FunctionalDependency::getLhsAttrSet() {
	AttributeSet lhsAttr(lhs);
	return lhsAttr;
}

AttributeSet FunctionalDependency::getRhsAttrSet() {
	AttributeSet rhsAttr(rhs);
	return rhsAttr;
}

string FunctionalDependency::display() {
	
	FunctionalDependency fd = *this;
	set<int> lhs = fd.getLhs();
	set<int> rhs = fd.getRhs();
	string lhsStr;
	for (auto iter = lhs.begin(); iter != lhs.end(); ++iter) {
		lhsStr += std::to_string(static_cast<long long>(*iter));
	}
	string rhsStr;
	for (auto iter = rhs.begin(); iter != rhs.end(); ++iter) {
		rhsStr += std::to_string(static_cast<long long>(*iter));
	}

	string fullStr = lhsStr + "->" + rhsStr;

	return fullStr;
	
}