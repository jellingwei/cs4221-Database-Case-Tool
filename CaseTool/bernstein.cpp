#include <cassert>
#include <exception>
#include <algorithm>
#include <vector>
#include <utility>

#include "bernstein.h"



namespace bernstein {
	using std::exception;
	using std::string;
	using std::vector;
	using std::pair;
	using std::multimap;

	vector<string> message;
	vector<std::pair<int, FunctionalDependency> > step1Output;
	vector<FunctionalDependency > step2Output;

	vector<std::pair<int, FunctionalDependency> > getMessageForStep1() {
		return step1Output;
	}
	vector<FunctionalDependency > getMessageForRemovingTransitiveDependencies() {
		return step2Output;
	}

	/*
	 * For every FD X -> Y, produce FDs X -> a for every attribute a that is in Y.
	 */
	set<FunctionalDependency> bernstein::decomposeFd(set<FunctionalDependency> startFds) {
		set<FunctionalDependency> finalSet;

		for (auto iter = startFds.begin(); iter != startFds.end(); ++iter) {
			FunctionalDependency fd = *iter;

			set<int> rightAttributes = fd.getRhs();

			for (auto attrIter = rightAttributes.begin(); attrIter != rightAttributes.end(); ++attrIter) {
				set<int> newRhs;
				int attribute = *attrIter;
				newRhs.insert(attribute);

				FunctionalDependency newFd(fd.getLhs(), newRhs); 

				assert((newFd.getRhs()).size() == 1);
				finalSet.insert(newFd);
			}
		}

		return finalSet;
	}


	/**
	 * Returns an AttributeSet without the attribute attrToDrop
	 */
	AttributeSet dropAttributeFromAttributeSet(AttributeSet attrSet, int attrToDrop) {
		set<int> attributesInAttrSet = attrSet.getAttributes();
		attributesInAttrSet.erase(attrToDrop);

		return AttributeSet(attributesInAttrSet);
	}

	/**
	 * Detects redundant attributes in functional dependencies and removes them
	 */
	set<FunctionalDependency> bernstein::removeRedundantAttributes(set<FunctionalDependency> fdSet) {
		step1Output.clear();

		// first remove the trivial FDs
		fdSet = removeTrivialDependencies(fdSet);
		fdSet = decomposeFd(fdSet);
		set<FunctionalDependency> resultingFdSet = fdSet;

		for (auto iter = fdSet.begin(); iter != fdSet.end(); ++iter) {
			FunctionalDependency fd = *iter;

			AttributeSet lhs = AttributeSet(fd.getLhs());

			set<int> attrsOnLhs = lhs.getAttributes();
			for (auto attrIter = attrsOnLhs.begin(); attrIter != attrsOnLhs.end(); ++attrIter) {
				int attribute = *attrIter;

				// test if attribute is redundant
				// get closure using updated set of FDs
				AttributeSet withoutAttr = dropAttributeFromAttributeSet(lhs, attribute);
				AttributeSet closure = withoutAttr.getAttributeClosure(resultingFdSet);

				// check if the dropped attribute is inside the closure
				set<int> attributeAsSet;  attributeAsSet.insert(attribute);
				bool isRedundant = closure.containsAttributes(attributeAsSet);

				if (isRedundant) {
					step1Output.push_back(pair<int, FunctionalDependency>(attribute, fd));
					lhs = dropAttributeFromAttributeSet(lhs, attribute);
				}
			}

			resultingFdSet.erase(fd);
			FunctionalDependency newFd(lhs.getAttributes(), fd.getRhs());
			resultingFdSet.insert(newFd);
		}

		return resultingFdSet;
	}

	/**
	 * Constructs a minimal cover by removing redundant attributes and funcitonal dependencies
	 * @sa removeRedundantAttributes
	 * @sa eliminateTransitiveDependencies
	 */
	set<FunctionalDependency> obtainMinimalCover(set<FunctionalDependency> fdSet) {
		fdSet = decomposeFd(fdSet);
		fdSet = removeRedundantAttributes(fdSet);
		fdSet = eliminateTransitiveDependencies(fdSet);

		return fdSet; 
	}

	/**
	 * Returns a mapping of attributeset to functional dependencies with the attributeSet as its left-hand-side
	 * That is to say, maps X to a set of {X -> Y}
	 */
	unordered_map<AttributeSet, set<FunctionalDependency> > partitionFd(set<FunctionalDependency> fdSet) {
		unordered_map<AttributeSet, set<FunctionalDependency> > partition;

		for (auto iter = fdSet.begin(); iter != fdSet.end(); ++iter) {
			FunctionalDependency fd = *iter;

			AttributeSet lhs(fd.getLhs());
			
			if (partition.count(AttributeSet(fd.getLhs()))) {
				set<FunctionalDependency>& setOfFd = partition.at(fd.getLhs());
				setOfFd.insert(fd);
			} else {
				set<FunctionalDependency> newSet;
				newSet.insert(fd);
				partition[AttributeSet(fd.getLhs())] = newSet;
			}
		}
		
		return partition;
	}

	/**
	 * For every functional dependency, X -> Y, in initialFdSet, if Y contains an attribute from attrsToDrop,
	 * remove that attribute from Y.
	 */
	set<FunctionalDependency> removeAttributesOfEquivalentKeys(set<FunctionalDependency> initialFdSet, set<int> attrsToDrop) {
		set<FunctionalDependency> newFdSet;
		// iterate through all the initial function dependencies
		for (auto fdIter = initialFdSet.begin(); fdIter != initialFdSet.end(); ++fdIter) {
			FunctionalDependency curFd = *fdIter;
			// update the rhs of the functional dependency by dropping all attrs from attrsToDrop
			AttributeSet fdRhs = AttributeSet(curFd.getRhs());

			for (auto attrIter = attrsToDrop.begin(); attrIter != attrsToDrop.end(); ++attrIter) {
				int attr = *attrIter;
				fdRhs = dropAttributeFromAttributeSet(fdRhs, attr);
			}

			if (!fdRhs.getAttributes().empty()) {
				FunctionalDependency newFd = FunctionalDependency(curFd.getLhs(), fdRhs);
				newFdSet.insert(newFd);
			}
		}
		return newFdSet;
	}

	// comparator for sorting by size of LHS attributeSet
	bool sortByLhsSize (pair<AttributeSet, set<FunctionalDependency> >& pair1, pair<AttributeSet, set<FunctionalDependency> >& pair2) { 
		return pair1.first.size() < pair2.first.size(); 
	}


	vector<pair<AttributeSet, set<FunctionalDependency> > > sortPartitionBySizeOfLHS( unordered_map<AttributeSet, set<FunctionalDependency> > partitions ) {
		vector<pair<AttributeSet, set<FunctionalDependency> > > sortedList;

		for (auto iter = partitions.begin(); iter != partitions.end(); ++iter) {
			AttributeSet attrSet = iter->first;
			set<FunctionalDependency> fdSet = iter->second;

			sortedList.push_back(std::make_pair<AttributeSet, set<FunctionalDependency> >(attrSet, fdSet));
		}

		std::sort(sortedList.begin(), sortedList.end(), sortByLhsSize);

		return sortedList;

	}

	/**
	 * Detects equivalent keys in a partition by computing closures. 
	 * X and A are equivalent if X* contains A and A* contains X.
	 * The partition J, which contains X->A and A->X if X and A are equivalent, 
	 * is obtained from the mapping by using an empty attribute set as a key. (questionable decision. might need to redo)
	 *
	 */
	unordered_map<AttributeSet, set<FunctionalDependency> > mergeEquivalentKeys(unordered_map<AttributeSet, set<FunctionalDependency> > partitions, set<FunctionalDependency> allFds) {
		unordered_map<AttributeSet, bool > isAdded;
		unordered_map<AttributeSet, set<FunctionalDependency> > finalPartition;

		set<int> emptySet;
		AttributeSet emptyAttrSet(emptySet); // going to use this as J

		vector<pair<AttributeSet, set<FunctionalDependency> > > sortedPartitions = sortPartitionBySizeOfLHS(partitions);
		
		multimap<AttributeSet, AttributeSet> mapOfMergedLhsToMergedRhs;

		for (auto iter = sortedPartitions.begin(); iter != sortedPartitions.end(); ++iter) {
			AttributeSet attrSet = iter->first;
			set<FunctionalDependency> fdSet = iter->second;
			if (isAdded.count(attrSet) != 0 && isAdded[attrSet]) {
				continue;
			}
			isAdded[attrSet] = true;

			AttributeSet closure = attrSet.getAttributeClosure(allFds);

			for (auto iter2 = iter; iter2 != sortedPartitions.end(); ++ iter2) {
				if (iter == iter2) {  // @todo I don't know how to initialise iter2 to be iter + 1
					continue; 
				}
				AttributeSet attrSet2 = iter2->first;
				set<FunctionalDependency> fdSet2 = iter2->second;
				AttributeSet closure2 = attrSet2.getAttributeClosure(allFds);

				
				// if attrSet and attrSet2 are equivalent
				bool isEquivalent = (closure.containsAttributes(attrSet2.getAttributes()) && 
									 closure2.containsAttributes(attrSet.getAttributes()));


				bool isRhsBijectionWithSubsetOfAttrSetA = false;
				std::pair <multimap<AttributeSet,AttributeSet>::iterator, multimap<AttributeSet,AttributeSet>::iterator> rhs 
					= mapOfMergedLhsToMergedRhs.equal_range(attrSet);
				
				for (auto mapIter = rhs.first; mapIter != rhs.second; ++mapIter) {
					AttributeSet possibleSubset = mapIter->second;
					if (attrSet.containsAttributes(possibleSubset)) {
						isRhsBijectionWithSubsetOfAttrSetA = true;
					}
				}

				// if properly equivalent 
				if (isEquivalent && !isRhsBijectionWithSubsetOfAttrSetA) {

					mapOfMergedLhsToMergedRhs.insert(pair<AttributeSet, AttributeSet>(attrSet, attrSet2));
					mapOfMergedLhsToMergedRhs.insert(pair<AttributeSet, AttributeSet>(attrSet2, attrSet));

					isAdded[attrSet2] = true;
					FunctionalDependency fd1(attrSet, attrSet2);
					FunctionalDependency fd2(attrSet2, attrSet);
					
					// add attrSet->attrSet2 and attrSet2->attrSet into J
					set<FunctionalDependency> equivalentFdSet;
					if (finalPartition.count(emptyAttrSet) != 0) {
						equivalentFdSet = finalPartition[emptyAttrSet];
					}
					equivalentFdSet.insert(fd1);
					equivalentFdSet.insert(fd2); 
					finalPartition[emptyAttrSet] = equivalentFdSet;

					// update fds
					// From the notes, this is the step for:
					//if X -> Z in H  and  Z is in Y,  then delete  X -> Z  from H.
					//Similarly, if Y ->Z in H and Z in X,  then delete Y -> Z from H.
					fdSet = removeAttributesOfEquivalentKeys(fdSet, attrSet2.getAttributes());
					fdSet2 = removeAttributesOfEquivalentKeys(fdSet2, attrSet.getAttributes());

					// merge the two fdSets together into fdSet
					fdSet.insert(fdSet2.begin(), fdSet2.end());

					// Note: since isAdded[fdSet2] is set to true earlier, this prevents fdSet2 from getting inserted later on.

				}
			}
			finalPartition[attrSet] = fdSet;
		}

		return finalPartition;
	}

	set<FunctionalDependency> dropFdFromSet(set<FunctionalDependency> fdSet, FunctionalDependency fd) {
		fdSet.erase(fd);
		return fdSet;
	}

	set<FunctionalDependency> addFdToSet(set<FunctionalDependency> fdSet, FunctionalDependency fd) {
		fdSet.insert(fd);
		return fdSet;
	}

	/**
	 * Removes redundant dependencies
	 */
	set<FunctionalDependency> eliminateTransitiveDependencies(set<FunctionalDependency> startingFdSet) {
		step2Output.clear();
		set<FunctionalDependency> currentFdSet = startingFdSet;
		for (auto fdIter = startingFdSet.begin(); fdIter != startingFdSet.end(); ++fdIter) {
			FunctionalDependency currentFd = *fdIter;

			// check if currentFd is redundant
			// 1. create a set of fd without currentFd
			set<FunctionalDependency> possibleFdSet = dropFdFromSet(currentFdSet, currentFd);
			AttributeSet lhs = currentFd.getLhs();

			// 2. and check if the closure of fd's lhs can still get the rhs
			AttributeSet attrClosure = lhs.getAttributeClosure(possibleFdSet);
			bool isRedundant = attrClosure.containsAttributes(currentFd.getRhs());

			if (isRedundant) {
				step2Output.push_back(currentFd);
				currentFdSet = possibleFdSet;
			}
		}

		return currentFdSet;
	}

	set<FunctionalDependency> createSetOfFDFromPartitions(unordered_map<AttributeSet, set<FunctionalDependency> > partitions) {
		set<FunctionalDependency> fdSet;
		for (auto iter = partitions.begin(); iter != partitions.end(); ++iter) {
			set<FunctionalDependency> fdInPartition = iter->second;

			fdSet.insert(fdInPartition.begin(), fdInPartition.end());
		}
		return fdSet;
	}


	unordered_map<AttributeSet, set<FunctionalDependency> > bernstein::addFdInJBackToCorrespondingGroup(unordered_map<AttributeSet, set<FunctionalDependency> > partitions) {
		// In the notes, this is for step 5
		// need to first add each FD in J to its corresponding group
		set<int> emptySet; AttributeSet equivalentAttrSet(emptySet);
		if (partitions.count(equivalentAttrSet) != 0) {
			set<FunctionalDependency> jFd = partitions[equivalentAttrSet];

			// for every FD in the set J, try to find a corresponding group to add the FD back
			for (auto fdIter = jFd.begin(); fdIter != jFd.end(); ++fdIter) {
				FunctionalDependency fd = *fdIter;

				AttributeSet lhs = fd.getLhs();
				AttributeSet rhs = fd.getRhs();
				bool fdHasCorrespondingGroupInLhs = (partitions.count(lhs) != 0);
				if (fdHasCorrespondingGroupInLhs) {
					partitions[equivalentAttrSet] = dropFdFromSet(partitions[equivalentAttrSet], fd);
					partitions[lhs] = addFdToSet(partitions[lhs], fd);
				} else {
					bool fdHasCorrespondingGroupInRhs = (partitions.count(rhs) != 0);
					if (fdHasCorrespondingGroupInRhs) {
						partitions[equivalentAttrSet] = dropFdFromSet(partitions[equivalentAttrSet], fd);
						partitions[rhs] = addFdToSet(partitions[rhs], fd);
					}

				}
			}
		}
		return partitions;
	}

	/**
	 * The second parameter is the result from createSetOfFDFromPartitions.
	 * 
	 */
	unordered_map<AttributeSet, set<FunctionalDependency> > eliminateTransitiveDependenciesForPartition(unordered_map<AttributeSet, set<FunctionalDependency> > partitions, set<FunctionalDependency> allFd) {
		step2Output.clear();
		set<FunctionalDependency> currentFdSet = allFd;
		unordered_map<AttributeSet, set<FunctionalDependency> > finalPartitions = partitions;

		// iterate over all FDs, when a FD is redundant, iterate over all partitions and drop it if its found
		// inefficient but it should work
		// @todo speed up
		for (auto fdIter = allFd.begin(); fdIter != allFd.end(); ++fdIter) {
			FunctionalDependency currentFd = *fdIter;

			// check if currentFd is redundant
			// 1. create a set of fd without currentFd
			set<FunctionalDependency> possibleFdSet = dropFdFromSet(currentFdSet, currentFd);
			AttributeSet lhs = currentFd.getLhs();

			// 2. and check if the closure of fd's lhs can still get the rhs
			AttributeSet attrClosure = lhs.getAttributeClosure(possibleFdSet);
			bool isRedundant = attrClosure.containsAttributes(currentFd.getRhs());
			

			if (isRedundant) {
				currentFdSet = possibleFdSet; // drop from full set of FD

				for (auto partitionIter = partitions.begin(); partitionIter != partitions.end(); ++partitionIter) {
					set<FunctionalDependency> fdsInPartition = partitionIter->second;
					fdsInPartition = dropFdFromSet(fdsInPartition, currentFd);
					finalPartitions[partitionIter->first] = fdsInPartition;

				}
			}

			partitions = finalPartitions;
		}

		return partitions;
	}

	
	
	set<std::pair<AttributeSet, set<AttributeSet> > >  constructRelations(unordered_map<AttributeSet, set<FunctionalDependency> > partitions) {
		set<std::pair<AttributeSet, set<AttributeSet> > > finalAnswer;


		// construct relations
		for (auto iter = partitions.begin(); iter != partitions.end(); ++iter) {
			AttributeSet attrSet = iter->first;
			set<FunctionalDependency> fdSet = iter->second; 
			
			AttributeSet allAttr(attrSet);
			set<AttributeSet> allKeys;

			// collect all attributes in fdSet into allAttr, collect all lhs of fd as keys
			for (auto iter = fdSet.begin(); iter != fdSet.end(); ++iter) {
				set<int> attributesInAllAttr = allAttr.getAttributes();

				FunctionalDependency fd = *iter;
				allKeys.insert(AttributeSet(fd.getLhs()));

				set<int> lhs = fd.getLhs();
				for (auto attrIter = lhs.begin(); attrIter != lhs.end(); ++attrIter) {
					attributesInAllAttr.insert(*attrIter);
				}
				
				set<int> rhs = fd.getRhs();
				for (auto attrIter = rhs.begin(); attrIter != rhs.end(); ++attrIter) {
					attributesInAllAttr.insert(*attrIter);
				}
				
				allAttr = AttributeSet(attributesInAllAttr);		
			}

			if (allAttr.size() > 0) {
				finalAnswer.insert(std::make_pair<AttributeSet, set<AttributeSet> >(allAttr, allKeys));
			}
		}

		return finalAnswer;
	}


	set<FunctionalDependency> removeTrivialDependencies(set<FunctionalDependency> fdSet) {
		set<FunctionalDependency> resultingSet;

		for (auto iter = fdSet.begin(); iter != fdSet.end(); ++iter) {
			FunctionalDependency fd = *iter;

			set<int> lhsAttributes = fd.getLhs();
			AttributeSet rhsAttributes = AttributeSet(fd.getRhs());

			// remove, from the righthand-side, attributes that are on the left side
			for (auto attrIter = lhsAttributes.begin(); attrIter != lhsAttributes.end(); ++attrIter) {
				int attr = *attrIter;

				rhsAttributes = dropAttributeFromAttributeSet(rhsAttributes, attr);
			}

			if (!rhsAttributes.getAttributes().empty()) {
				fd = FunctionalDependency(AttributeSet(lhsAttributes), rhsAttributes);
				resultingSet.insert(fd);
			}
		}

		return resultingSet;
	}


	AttributeSet getAllAttributesInRelations(set<std::pair<AttributeSet, set<AttributeSet> > > relations) {
		set<int> attrInRelations;

		for (auto iter = relations.begin(); iter != relations.end(); ++iter) {
			AttributeSet attributesSet = iter->first;
			set<int> attrs = attributesSet.getAttributes();

			attrInRelations.insert(attrs.begin(), attrs.end());
		}

		return attrInRelations;
	}

	set<AttributeSet> findCandidateKeys(AttributeSet attributes, set<FunctionalDependency> allFds) {
		normalTest::resetState();
		
		qDebug() << QString(attributes.toString().c_str());

		int status = normalTest::findCandidateKeys(allFds, attributes);
		set<AttributeSet> candidateKeys = normalTest::getCandidateKeys();

		string attrsStr;
		
		qDebug() <<"num of keys found?";
		qDebug() << QString((std::to_string(static_cast<long long>(status)).c_str()));
		

		return candidateKeys;
	}


	std::pair<AttributeSet, set<AttributeSet> >  constructMissingAttrRelation(set<std::pair<AttributeSet, set<AttributeSet> > > relations, int sizeOfAttrs, AttributeSet key) {
		set<int> missingAttr;
		for (int i = 0; i < sizeOfAttrs; i++) {
			missingAttr.insert(i);
		}

		set<int> allAttrInRel = getAllAttributesInRelations(relations).getAttributes();

		for (auto iter = allAttrInRel.begin(); iter != allAttrInRel.end(); ++iter) {
			missingAttr.erase(*iter);
		}
		
		// make a relation with the missing attrs and the key 
		set<int> keyAndMissingAttributes = missingAttr;
		set<int> keyAttrs = key.getAttributes();
		keyAndMissingAttributes.insert(keyAttrs.begin(), keyAttrs.end());

		// relation contains the missing attributes and the key
		AttributeSet keyAndMissingAttributesSet = AttributeSet(keyAndMissingAttributes);
		// only one key for the relation, which is the key
		set<AttributeSet> keySet;
		keySet.insert(key);

		return std::make_pair<AttributeSet, set<AttributeSet> >(keyAndMissingAttributesSet, keySet);
	}

}

