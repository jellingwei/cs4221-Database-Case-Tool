#include <cassert>
#include <exception>

#include "bernstein.h"



namespace bernstein {
	using std::exception;

	/*
	 * For every FD X -> Y, produce FDs X -> a for every attribute a that is in Y.
	 */
	set<FunctionalDependency> decomposeFd(set<FunctionalDependency> startFds) {
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



	set<FunctionalDependency> removeRedundantAttributes(set<FunctionalDependency> fdSet, AttributeSet allAttributes) {
		return fdSet;
		
	}

	set<FunctionalDependency> obtainMinimalCover(set<FunctionalDependency> fdSet) {
		return fdSet; 
	}

	unordered_map<AttributeSet, set<FunctionalDependency> > partitionFd(set<FunctionalDependency> fdSet) {

		throw exception();
	
	}

	unordered_map<AttributeSet, set<FunctionalDependency> > mergeEquivalentKeys(set<FunctionalDependency> fdSet) {

		throw exception();
	}

	unordered_map<AttributeSet, set<FunctionalDependency> > eliminateTransitiveDependencies(set<FunctionalDependency> fdSet) {
		throw exception();

	}

	set<AttributeSet> constructRelations(unordered_map<AttributeSet, set<FunctionalDependency> >) {
		throw exception();
	}
}

