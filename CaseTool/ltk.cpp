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
	using std::pair;
	
	//Private attributes
	unordered_map<AttributeSet, set<FunctionalDependency> > globalSynthesizedFDs;
	set<FunctionalDependency> globalFDs;

	//Private methods
	set<Relation> runBernstein(set<FunctionalDependency> startFd, int numAttributes);
	AttributeSet getSmallestKey(set<AttributeSet> candidateKeys);
	set<Relation> augmentPreparatorySchema(set<Relation> constructedRelations, set<FunctionalDependency> minimalFDs, AttributeSet startAttributes);
	set<FunctionalDependency> constructSynthesizedPrimeFDs(AttributeSet relAttr, AttributeSet excludeAttr);
	set<FunctionalDependency> getGlobalFDs();

	set<Relation> constructPreparatorySchema(set<FunctionalDependency> startFDs, int numAttributes) {
		return runBernstein(startFDs, numAttributes);
	}

	//Outputs the new set of synthesized key of relation if attribute is superfluous
	//Otherwise, output an empty attribute set
	set<AttributeSet> superfluousAttributeDetection(Relation preparatoryRelation, AttributeSet singleAttribute) {
		bool isSuperfluous = true;
		AttributeSet prepRelationAttributes = preparatoryRelation.getAttributes();
		set<AttributeSet> prepRelationKeys = preparatoryRelation.getKeys();
		set<AttributeSet> reducedSynthesizedKeys;
		set<FunctionalDependency> reducedSynthesizedFDs;
		
		//Take all synthesized keys except those with the single attribute
		for (auto itr = prepRelationKeys.begin(); itr != prepRelationKeys.end(); ++itr) {
			AttributeSet synthesizedKey = *itr;
			if (!synthesizedKey.containsAttributes(singleAttribute)) {
				reducedSynthesizedKeys.insert(synthesizedKey);
			}
		}
				
		if (reducedSynthesizedKeys.size() == 0) {
			isSuperfluous = false;
		} else {
			isSuperfluous = false;
			//Construct a new set of synthesized FDs without the single attribute
			reducedSynthesizedFDs = constructSynthesizedPrimeFDs(prepRelationAttributes, singleAttribute);

			for (auto keyItr = reducedSynthesizedKeys.begin(); keyItr != reducedSynthesizedKeys.end(); ++keyItr) {
				AttributeSet key = *keyItr;
				if (key.getAttributeClosure(reducedSynthesizedFDs).containsAttributes(singleAttribute)) {
					isSuperfluous = true;
				}
			}
		}

		if (!isSuperfluous) {
			set<AttributeSet> empty;
			return empty;
		}

		set<AttributeSet> lostKeys = prepRelationKeys;
		for (auto keyItr = reducedSynthesizedKeys.begin(); keyItr != reducedSynthesizedKeys.end(); ++keyItr) {
			lostKeys.erase(*keyItr);  //Get all keys that contains the single attribute
		}
		
		for (auto itr = lostKeys.begin(); itr != lostKeys.end(); ++itr) {
			AttributeSet key = *itr;
			AttributeSet keyClosure = key.getAttributeClosure(reducedSynthesizedFDs);
			if (keyClosure.containsAttributes(prepRelationAttributes)) {
				continue;  //Skip for this key
			} else {
				AttributeSet reducedLHS = keyClosure.intersect(prepRelationAttributes) - singleAttribute;
				AttributeSet globalClosure = reducedLHS.getAttributeClosure(getGlobalFDs());

				if (globalClosure.containsAttributes(prepRelationAttributes)) {
					return reducedSynthesizedKeys;
					/*for (auto keyItr = prepRelationKeys.begin(); keyItr != prepRelationKeys.end(); ++keyItr) {
						AttributeSet initialKey = *keyItr;
						if (globalClosure.containsAttributes(initialKey)) {
							reducedSynthesizedKeys.insert(initialKey);
							return reducedSynthesizedKeys;
						}
					}*/
				} else {
					isSuperfluous = false;
					break;
				}
			}
		}

		if (isSuperfluous) {
			return reducedSynthesizedKeys;
		} else {
			set<AttributeSet> empty;
			return empty;
		}
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
		globalSynthesizedFDs.clear();
		
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
			globalSynthesizedFDs[attributes] = FDs;
		}
		return globalSynthesizedFDs;
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

	AttributeSet fullAttributeSet(int numAttributes) {
		set<int> fullAttr;
		for (int i = 0; i < numAttributes; i++) {
			fullAttr.insert(i);
		}
		return fullAttr;
	}

	set<Relation> runBernstein(set<FunctionalDependency> startFd, int numAttributes) {
		set<Relation> finalRelations;

		//Run Bernstein algorithm
		set<FunctionalDependency> newFdSet = bernstein::removeRedundantAttributes(startFd);  //Step 1
		set<FunctionalDependency> minimalCover = bernstein::obtainMinimalCover(startFd);  //Step 2
		unordered_map<AttributeSet, set<FunctionalDependency> > partitions = bernstein::partitionFd(minimalCover);  //Step 3
		partitions = bernstein::mergeEquivalentKeys(partitions, minimalCover);  // step 4

		set<FunctionalDependency> allFdAfterPartitioning = bernstein::createSetOfFDFromPartitions(partitions);  // step 5
		partitions = bernstein::eliminateTransitiveDependenciesForPartition(partitions, allFdAfterPartitioning);  // step 5
		partitions = bernstein::addFdInJBackToCorrespondingGroup(partitions);
		set<std::pair<AttributeSet, set<AttributeSet> > > finalAnswer = bernstein::constructRelations(partitions);  // step 6

		set<AttributeSet> keysForRelation;  // set of attributes that form key for at least one relation

		// step 7 : find all keys for every relation
		for (auto iter = finalAnswer.begin(); iter != finalAnswer.end(); ++iter) {
			// extension: find all keys of the relation
			AttributeSet attrSet = iter->first;
			set<AttributeSet> candidateKeys = bernstein::findCandidateKeys(attrSet, allFdAfterPartitioning);
			keysForRelation.insert(candidateKeys.begin(), candidateKeys.end());

			set<AttributeSet> keys = iter->second;
			Relation relation(attrSet, keys);
			finalRelations.insert(relation);
		}

		// step 8
		set<AttributeSet> candidateKeys = bernstein::findCandidateKeys(fullAttributeSet(numAttributes), allFdAfterPartitioning);

		AttributeSet smallestKey = getSmallestKey(candidateKeys);
		set<int> attrs = smallestKey.getAttributes();

		std::pair<AttributeSet, set<AttributeSet> > extraRelation = bernstein::constructMissingAttrRelation(finalAnswer, numAttributes, smallestKey);

		// check if the extra relation is a key to some relation
		if (keysForRelation.count(extraRelation.first) == 0) {
			AttributeSet attrSet = extraRelation.first;
			set<AttributeSet> keys = extraRelation.second;
			Relation relation(attrSet, keys);
			finalRelations.insert(relation);
		}
		return finalRelations;
	}

	AttributeSet getSmallestKey(set<AttributeSet> candidateKeys) {
		AttributeSet answer;
		bool isSet = false;

		for (auto iter = candidateKeys.begin(); iter != candidateKeys.end(); ++iter) {
			if (!isSet || iter->getAttributes().size() < answer.getAttributes().size()) {
				answer = *iter;
				isSet = true;
			}
		}

		return answer;

	}
}
