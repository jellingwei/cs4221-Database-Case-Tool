#pragma once

#include "functionaldependency.h"
#include "attributeset.h"
#include <set>
#include <algorithm>

namespace normalTest {
	using std::set;
	using std::string;

	
	//maybe a set of non primes


	int findCandidateKeys(set<FunctionalDependency>, AttributeSet);
	int rootFoo(set<FunctionalDependency>, AttributeSet);
	char runNormalTests(set<FunctionalDependency>, AttributeSet attr);
	char secondNormalForm(set<FunctionalDependency>, AttributeSet attr);
	char thirdNormalForm(set<FunctionalDependency>);
	char boyceCoddNormalForm(set<FunctionalDependency>);
	char elementaryKeyNormalForm(set<FunctionalDependency>);
	int findFactorial(int num);
	set<int> findNonPrime(set<int> attributes);

}
