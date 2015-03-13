#pragma once

#include <set>
using std::set;

class Relation {
public:
	Relation(set<int> attributes, set<int> keys);

	int getRelationSize() const;
	int getKeySize() const;

	set<int> getAttributes();
	set<int> getKeys();

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
	set<int> keys;
};
