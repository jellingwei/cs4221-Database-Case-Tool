#pragma once

#include <set>
using std::set;

class Relation {
public:
	Relation(set<int> attributes, set<int> keys);

	int getRelationSize() const;
	int getKeySize() const;

	set<int> getAttributes();
	set<set<int>> getKeys();

	bool insertKey(set<int> key);  //Returns true if key is already in the relation

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
