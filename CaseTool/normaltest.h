#pragma once
#include "functionaldependency.h"
#include "attributeset.h"
#include <set>
#include <algorithm>

namespace normalTest {
	using std::set;
	using std::string;

	set<AttributeSet> candidateKeys;
	set<AttributeSet> powerSet;
	//maybe a set of non primes


	int findCandidateKeys(set<FunctionalDependency>, AttributeSet);
	void rootFoo(set<FunctionalDependency>, AttributeSet);
	char runNormalTests(set<FunctionalDependency>);
	char secondNormalForm(set<FunctionalDependency>);
	char thirdNormalForm(set<FunctionalDependency>);
	char boyceCoddNormalForm(set<FunctionalDependency>);
	char elementaryKeyNormalForm(set<FunctionalDependency>);

}