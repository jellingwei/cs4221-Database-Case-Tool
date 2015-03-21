#include <exception>
#include <unordered_map>
#include <vector>

#include "ltk.h"
#include "bernstein.h"

namespace ltk {
	using std::set;
	using std::vector;
	using std::exception;
	using std::unordered_map;
	using std::make_pair;
	using std::swap;
	
	//Private attributes
	unordered_map<AttributeSet, set<FunctionalDependency> > globalSynthesizedFDs;
	set<FunctionalDependency> globalFDs;

	//Private methods
	set<Relation> augmentPreparatorySchema(set<Relation> constructedRelations, set<FunctionalDependency> minimalFDs, AttributeSet startAttributes);
	set<FunctionalDependency> constructSynthesizedPrimeFDs(AttributeSet relAttr, AttributeSet excludeAttr);
	set<FunctionalDependency> getGlobalFDs();

	set<Relation> constructPreparatorySchema(AttributeSet startAttributes, set<FunctionalDependency> startFDs) {
		set<FunctionalDependency> singletonFDs = bernstein::decomposeFd(startFDs);
		set<FunctionalDependency> nonRedundantAttributesFDs = bernstein::removeRedundantAttributes(singletonFDs);
		set<FunctionalDependency> minimalFDs = bernstein::eliminateTransitiveDependencies(nonRedundantAttributesFDs);
		unordered_map<AttributeSet, set<FunctionalDependency>> partitionedFDs = bernstein::partitionFd(minimalFDs);
		set<Relation> finalRelation;

		for (auto mapItr = partitionedFDs.begin(); mapItr != partitionedFDs.end(); ++mapItr) {
			AttributeSet attributes = mapItr->first;
			set<FunctionalDependency> fdSet = mapItr->second;
			
			for (auto fdItr = fdSet.begin(); fdItr != fdSet.end(); ++fdItr) {
				FunctionalDependency fd = *fdItr;
				AttributeSet key = fd.getLhsAttrSet();
				Relation relation(attributes, key);
				finalRelation.insert(relation);
			}
		}
		return finalRelation;
		//Construct relations
		//Augment preparatory schema(constructed relations, minimalFDs, startAttributes);
	}

	set<Relation> augmentPreparatorySchema(set<Relation> constructedRelations, set<FunctionalDependency> minimalFDs, AttributeSet startAttributes) {
		//For each constructed relation, check if the closure of attributes can give back the whole initial set of attributes
		for (auto itr = constructedRelations.begin(); itr != constructedRelations.end(); ++itr) {
			Relation relation = *itr;
			set<AttributeSet> keySet = relation.getKeys();
			/*if (attributes.getAttributeClosure() != startAttributes) {
				//TODO: Find minimal subset
			}*/
		}
		//If cannot, construct a subset of the initial set of attributes such that its closure can give it back
		//Add this relation to the preparatory schema to give the whole preparatory schema
		throw exception();
	}

	//Outputs the new set of synthesized key of relation if attribute is superfluous
	//Otherwise, output an empty attribute set
	set<AttributeSet> superfluousAttributeDetection(Relation preparatoryRelation, AttributeSet singleAttribute) {
		//Mark attribute as superfluous
		bool isSuperfluous = true;
		set<int> singleAttributeSet = singleAttribute.getAttributes();
		int singleAttributeValue = *(singleAttributeSet.begin());
		AttributeSet prepRelationAttributes = preparatoryRelation.getAttributes();
		set<AttributeSet> prepRelationKeys = preparatoryRelation.getKeys();
		set<AttributeSet> reducedSynthesizedKeys;
		set<FunctionalDependency> reducedSynthesizedFDs;
		
		//Take all synthesized keys except those with the single attribute
		for (auto itr = prepRelationKeys.begin(); itr != prepRelationKeys.end(); ++itr) {
			AttributeSet synthesizedKey = *itr;
			if (!synthesizedKey.containsAttributes(singleAttributeSet)) {
				reducedSynthesizedKeys.insert(synthesizedKey);
			}
		}
		
		//Construct a new set of synthesized FDs without the single attribute
		reducedSynthesizedFDs = constructSynthesizedPrimeFDs(prepRelationAttributes, singleAttribute);
		
		if (reducedSynthesizedKeys.size() == 0) {
			isSuperfluous = false;
			set<AttributeSet> empty;
			return empty;
		}

		set<AttributeSet> lostKeys = prepRelationKeys;
		for (auto keyItr = reducedSynthesizedKeys.begin(); keyItr != reducedSynthesizedKeys.end(); ++keyItr) {
			lostKeys.erase(*keyItr);
		}
		
		isSuperfluous = false;  //Use this boolean to indicate if there is such a key where key determines singleAttribute
		for (auto itr = lostKeys.begin(); itr != lostKeys.end(); ++itr) {
			AttributeSet key = *itr;
			AttributeSet attrClosure = key.getAttributeClosure(reducedSynthesizedFDs);
			if (attrClosure == prepRelationAttributes) {
				continue;
			} else {
				AttributeSet reducedLHS = attrClosure.intersect(prepRelationAttributes) - singleAttribute;
				AttributeSet globalClosure = reducedLHS.getAttributeClosure(getGlobalFDs());
				if (globalClosure == prepRelationAttributes) {
					for (auto keyItr = prepRelationKeys.begin(); keyItr != prepRelationKeys.end(); ++keyItr) {
						AttributeSet initialKey = *keyItr;
						if (globalClosure.containsAttributes(initialKey)) {
							reducedSynthesizedKeys.insert(initialKey);
							return reducedSynthesizedKeys;
						}
					}
				} else {
					isSuperfluous = false;
					break;
				}
			}
		}

		set<AttributeSet> empty;
		return empty;
	}

	set<FunctionalDependency> getGlobalFDs() {
		if (globalFDs.size() != 0) {
			return globalFDs;
		} else {
			for (auto itr = globalSynthesizedFDs.begin(); itr != globalSynthesizedFDs.end(); ++itr) {
				set<FunctionalDependency> FD = itr->second;
				for (auto fdItr = FD.begin(); fdItr != FD.end(); ++fdItr) {
					globalFDs.insert(*fdItr);
				}
			}
			return globalFDs;
		}
	}

	unordered_map<AttributeSet, set<FunctionalDependency> > createSynthesizedFDs(set<Relation> relationSet) {	
		unordered_map<AttributeSet, set<FunctionalDependency>> newFDs;
		
		for (auto relItr = relationSet.begin(); relItr != relationSet.end(); ++relItr) {
			Relation relation = *relItr;
			AttributeSet attributes = relation.getAttributes();
			set<AttributeSet> keys = relation.getKeys();
			set<FunctionalDependency> FDs;

			for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr) {
				AttributeSet key = *keyItr;
				AttributeSet fdRHS(attributes - key);
				FunctionalDependency fd(key, fdRHS);
				FDs.insert(fd);
			}
			newFDs[attributes] = FDs;
		}
		return newFDs;
	}

	set<FunctionalDependency> constructSynthesizedPrimeFDs(AttributeSet relAttr, AttributeSet excludeAttr) {
		set<FunctionalDependency> synPrimeFDs;

		for (auto synItr = globalSynthesizedFDs.begin(); synItr != globalSynthesizedFDs.end(); ++synItr) {
			AttributeSet attributes = synItr->first;
			set<FunctionalDependency> fdSet = synItr->second;

			for (auto fdItr = fdSet.begin(); fdItr != fdSet.end(); ++fdItr) {
				FunctionalDependency fd = *fdItr;

				if (attributes == relAttr) {
					AttributeSet key = fd.getLhsAttrSet();

					if (!key.containsAttributes(excludeAttr)) {
						AttributeSet rhs = fd.getRhsAttrSet();
						rhs = rhs - excludeAttr;
						rhs = rhs - key;
						FunctionalDependency newFD(key, rhs);
						synPrimeFDs.insert(newFD);
					} else {
						continue;
					}
				} else {
					//Just insert the FDs if the attributes do not match
					synPrimeFDs.insert(fd);
				}
			}
		}
		return synPrimeFDs;
	}

	set<Relation> deletionNormalization(AttributeSet attributes, set<FunctionalDependency> FDs) {
		//Construct preparatory schema from above
		set<Relation> preparatoryRelationsSet = constructPreparatorySchema(attributes, FDs);
		vector<Relation> preparatoryRelations;
		for (auto relItr = preparatoryRelations.begin(); relItr != preparatoryRelations.end(); ++relItr) {
			Relation relation = *relItr;
			preparatoryRelations.push_back(relation);
		}
		
		set<Relation> finalRelation;
		//Create synthesized FDs
		globalSynthesizedFDs = createSynthesizedFDs(preparatoryRelationsSet);
		//For each relation, for each attribute, 
		for (unsigned int i = 0; i < preparatoryRelations.size(); i++) {
			AttributeSet finalAttributes;
			Relation relation = preparatoryRelations[i];
			AttributeSet attributes = relation.getAttributes();
			set<int> attributeSet = attributes.getAttributes();
			for (auto attributesItr = attributeSet.begin(); attributesItr != attributeSet.end(); ++attributesItr) {
				int singleAttributeValue = *attributesItr;
				set<int> singleSet;
				singleSet.insert(singleAttributeValue);
				AttributeSet singleAttribute(singleSet);;
				set<AttributeSet> returnedKeys = superfluousAttributeDetection(relation, singleAttribute);
				if (returnedKeys.size() != 0) {
					finalAttributes = attributes - singleAttribute;
				}
				Relation newRelation(finalAttributes, returnedKeys);
				swap(newRelation, preparatoryRelations[i]);
				i--;  //Re-run the algorithm on the new relation again
			}
		}
		//if superfluous attribute detection returns a nonempty set, construct a new relation and replace in preparatory schema
		//Final output is in LTK normal form
		set<Relation> finalRelations;
		for (unsigned int i = 0; i < preparatoryRelations.size(); i++) {
			finalRelations.insert(preparatoryRelations[i]);
		}
		return finalRelations;
	}
}
