#include <set>

#include "functionaldependency.h"
#include "attributeset.h"

using namespace std;


FunctionalDependency::FunctionalDependency() {

}

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

