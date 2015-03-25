#pragma once

#include "functionaldependency.h"
#include "attributeset.h"
#include <set>
#include <algorithm>

namespace normalTest {
	using std::set;
	using std::string;



	
	//maybe a set of non primes
	void resetState();
	set<AttributeSet> getCandidateKeys();

	int findCandidateKeys(set<FunctionalDependency>, AttributeSet);
	char normalTestRootProcess(set<FunctionalDependency>, AttributeSet);
	char runNormalTests(set<FunctionalDependency>, AttributeSet attr);
	char secondNormalForm(set<FunctionalDependency>, AttributeSet attr);
	char thirdNormalForm(set<FunctionalDependency>, AttributeSet attr);
	char boyceCoddNormalForm(set<FunctionalDependency>, AttributeSet attr);
	//char elementaryKeyNormalForm(set<FunctionalDependency>, AttributeSet attr);
	int findFactorial(int num);
	set<int> findNonPrime(set<int> attributes);
	set<int> findPrime();
	int isSubsetOf(set<int>, set<int>);
}
