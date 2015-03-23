#pragma once

#include <unordered_map>
#include <set>
#include <algorithm>
#include <QDebug> 


using std::set;
using std::min_element;
using std::string;

class AttributeSet;

class FunctionalDependency {
public:
	FunctionalDependency();
	FunctionalDependency(set<int>, set<int>);
	FunctionalDependency(AttributeSet, AttributeSet);

	AttributeSet getLhsAttrSet();
	AttributeSet getRhsAttrSet();
	set<int> getLhs();
	set<int> getRhs();

	bool operator<( const FunctionalDependency& fd2 ) const
	{
		if (lhs < fd2.lhs) {
			return true;
		} else if (lhs > fd2.lhs) {
			return false;
		} else if (lhs == fd2.lhs) {

			if (rhs < fd2.rhs) {
				return true;
			} else if (rhs > fd2.rhs) {
				return false;
			}
			return false;
		}
	}
	
	bool operator==( const FunctionalDependency& fd2 ) const
	{
		return (lhs == fd2.lhs) && (rhs == fd2.rhs);
	}

	string display();

private:
	set<int> lhs;
	set<int> rhs;

};