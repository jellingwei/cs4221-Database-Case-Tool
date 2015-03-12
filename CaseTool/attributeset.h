#pragma once

#include <set>
#include <functional>
#include <string>
#include "functionaldependency.h"



using std::set;

class FunctionalDependency;

class AttributeSet {
public:
	AttributeSet(set<int>);
	AttributeSet getAttributeClosure(set<FunctionalDependency>);
	bool isFdApplicable(FunctionalDependency);
	bool containsAttributes(set<int>);

	AttributeSet addAttributesToSet(AttributeSet);
	set<int> getAttributes() const;
	int size();

	static set<AttributeSet> constructRelations(set<FunctionalDependency>);

	bool operator<( const AttributeSet& attributeSet2 ) const
	{
		return (attributes < attributeSet2.attributes);
	}

	bool operator==( const AttributeSet& attributeSet2 ) const
	{

		return (attributes == attributeSet2.attributes);
	}



private:
	set<int> attributes;

};

namespace std {
	template <>
	  struct hash<AttributeSet>
	  {
		std::size_t operator()(const AttributeSet& as) const
		{
			set<int> attr = as.getAttributes();
			string attrStr;
			for (auto iter = attr.begin(); iter!= attr.end(); ++iter) {
				attrStr += to_string(static_cast<long long>(*iter));
			}

			hash<string> str_hash;
			return str_hash(attrStr);
		}
	  };
}