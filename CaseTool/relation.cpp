#include "relation.h"

Relation::Relation(set<int> attr, set<int> relKeys) {
	attributes = attr;
	keys = relKeys;
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

set<int> Relation::getKeys() {
	return keys;
}
