#include "relation.h"

Relation::Relation(set<int> attr, set<int> relKeys) {
	attributes = attr;
	keys.insert(relKeys);
}

int Relation::getRelationSize() const {
	return attributes.size();
}

int Relation::getKeySize() const {
	return keys.size();
}

set<int> Relation::getAttributes() {
	return attributes;
}

set<set<int>> Relation::getKeys() {
	return keys;
}

bool Relation::insertKey(set<int> key) {
	if (keys.count(key) == 1) {
		return true;
	} else {
		keys.insert(key);
		return false;
	}
}
