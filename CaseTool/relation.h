#pragma once

#include <set>
#include "attributeset.h"
using std::set;

class Relation {
public:
	Relation(AttributeSet attributes, AttributeSet key);
	Relation(AttributeSet attributes, set<AttributeSet> keys);

	int getRelationSize() const;
	int getKeySize() const;

	AttributeSet getAttributes();
	set<AttributeSet> getKeys();

	void insertKey(AttributeSet key);

	bool operator<( const Relation& relation2 ) const 
	{
		if (_attributes == relation2._attributes) {
			return _keys < relation2._keys;
			/*if (_keys.size() == relation2._keys.size()) {
				if (_keys.size() == 0) {
					return false;
				} else {
					return *(_keys.begin()) < *(relation2._keys.begin());
				}
			} else {
				return _keys.size() == relation2._keys.size();
			}*/
		} else {
			return (_attributes < relation2._attributes);
		}
	}

	bool operator==( const Relation& relation2 ) const 
	{
		return (_attributes == relation2._attributes && _keys == relation2._keys);
	}

private:
	set<int> _attributes;
	set<set<int>> _keys;
};
