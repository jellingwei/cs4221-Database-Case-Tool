#pragma once

#include <set>
#include <functional>
#include <string>
#include "functionaldependency.h"
#include "relation.h"

using std::set;

class FunctionalDependency;

class AttributeSet {
public:
	AttributeSet();
	AttributeSet(set<int> attributes);
	AttributeSet getAttributeClosure(set<FunctionalDependency>);
	bool isFdApplicable(FunctionalDependency);
	bool containsAttributes(set<int>);
	bool containsAttributes(AttributeSet);

	AttributeSet intersect(AttributeSet);
	AttributeSet addAttributesToSet(AttributeSet);
	set<int> getAttributes() const;
	int size();

	bool operator<( const AttributeSet& attributeSet2 ) const
	{
		return (attributes < attributeSet2.attributes);
	}

	bool operator==( const AttributeSet& attributeSet2 ) const
	{
		return (attributes == attributeSet2.attributes);
	}

	AttributeSet operator-( const AttributeSet& attributeSet2 ) const
	{
		set<int> beginAttributes = attributes;
		set<int> set2Attributes = attributeSet2.attributes;
		for (auto itr = set2Attributes.begin(); itr != set2Attributes.end(); ++itr) {
			beginAttributes.erase(*itr);
		}
		AttributeSet returnValue(beginAttributes);
		return returnValue;
	}
	
	//Use + operator to perform union
	AttributeSet operator+( const AttributeSet& attributeSet2 ) const
	{
		set<int> beginAttributes = attributes;
		set<int> set2Attributes = attributeSet2.attributes;
		for (auto itr = set2Attributes.begin(); itr != set2Attributes.end(); ++itr) {
			beginAttributes.insert(*itr);
		}
		AttributeSet returnValue(beginAttributes);
		return returnValue;
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
