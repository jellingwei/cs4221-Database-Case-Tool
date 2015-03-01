#pragma once

#include <unordered_map>
#include <set>
#include <algorithm>
#include <QDebug> 


using std::set;
using std::min_element;

class AttributeSet;

class FunctionalDependency {
public:
	FunctionalDependency(set<int>, set<int>);
	FunctionalDependency(AttributeSet, AttributeSet);

	set<int> getLhs();
	set<int> getRhs();

	bool operator<( const FunctionalDependency& fd2 ) const
	{

		if (lhs < fd2.lhs) {
			return true;
		}
		
		if (rhs < fd2.rhs) {
			return true;
		}

		return false;
	}

private:
	set<int> lhs;
	set<int> rhs;

};