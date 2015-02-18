#include <set>

#include "functionaldependency.h"

using namespace std;


FunctionalDependency::FunctionalDependency(set<int> leftSide, set<int> rightSide) {
	lhs = leftSide;
	rhs = rightSide;
}

set<int> FunctionalDependency::getLhs() {
	return lhs;
}

set<int> FunctionalDependency::getRhs() {
	return rhs;
}
