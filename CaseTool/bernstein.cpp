#include <cassert>
#include <exception>

#include "bernstein.h"



namespace bernstein {
	using std::exception;
	using std::string;

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



	AttributeSet dropAttributeFromAttributeSet(AttributeSet attrSet, int attrToDrop) {
		set<int> attributesInAttrSet = attrSet.getAttributes();
		attributesInAttrSet.erase(attrToDrop);

		return AttributeSet(attributesInAttrSet);
	}

	set<FunctionalDependency> bernstein::removeRedundantAttributes(set<FunctionalDependency> fdSet) {
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
					lhs = withoutAttr;
				}
			}

			resultingFdSet.erase(fd);
			FunctionalDependency newFd(lhs.getAttributes(), fd.getRhs());
			resultingFdSet.insert(newFd);
		}

		return resultingFdSet;
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

