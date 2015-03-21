#include "relation.h"

Relation::Relation(AttributeSet attr, AttributeSet relKey) {
	_attributes = attr.getAttributes();
	_keys.insert(relKey.getAttributes());
}

Relation::Relation(AttributeSet attr, set<AttributeSet> keys) {
	_attributes = attr.getAttributes();
	for (auto keysItr = keys.begin(); keysItr != keys.end(); ++keysItr) {
		AttributeSet key = *keysItr;
		_keys.insert(key.getAttributes());
	}
}

int Relation::getRelationSize() const {
	return _attributes.size();
}

int Relation::getKeySize() const {
	return _keys.size();
}

AttributeSet Relation::getAttributes() {
	AttributeSet returnValue(_attributes);
	return returnValue;
}

set<AttributeSet> Relation::getKeys() {
	set<AttributeSet> returnValue;
	for (auto itr = _keys.begin(); itr != _keys.end(); ++itr) {
		AttributeSet key(*itr);
		returnValue.insert(key);
	}
	return returnValue;
}

void Relation::insertKey(AttributeSet key) {
	set<int> keySet = key.getAttributes();
	_keys.insert(keySet);
}
