#include "relation.h"

Relation::Relation(AttributeSet attr, AttributeSet relKeys) {
	attributes = attr.getAttributes();
	keys.insert(relKeys.getAttributes());
}

int Relation::getRelationSize() const {
	return attributes.size();
}

int Relation::getKeySize() const {
	return keys.size();
}

AttributeSet Relation::getAttributes() {
	AttributeSet returnValue(attributes);
	return returnValue;
}

set<AttributeSet> Relation::getKeys() {
	set<AttributeSet> returnValue;
	for (auto itr = keys.begin(); itr != keys.end(); ++itr) {
		AttributeSet key(*itr);
		returnValue.insert(key);
	}
	return returnValue;
}

void Relation::insertKey(AttributeSet key) {
	set<int> keySet = key.getAttributes();
	keys.insert(keySet);
}
