#pragma once

#include <set>
#include "attributeset.h"
using std::set;

class Relation {
public:
	Relation(AttributeSet attributes, AttributeSet keys);

	int getRelationSize() const;
	int getKeySize() const;

	AttributeSet getAttributes();
	set<AttributeSet> getKeys();

	void insertKey(AttributeSet key);

	bool operator<( const Relation& relation2 ) const 
	{
		return (attributes < relation2.attributes && keys < relation2.keys);
	}

	bool operator==( const Relation& relation2 ) const 
	{
		return (attributes == relation2.attributes && keys == relation2.keys);
	}

private:
	set<int> attributes;
	set<set<int>> keys;
};
