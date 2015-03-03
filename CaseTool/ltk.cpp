#include <exception>
#include <unordered_map>

#include "ltk.h"
#include "bernstein.h"

namespace ltk {
	using std::set;
	using std::exception;
	using std::unordered_map;

	set<AttributeSet> deletionNormalization(AttributeSet, set<FunctionalDependency>) {
		throw exception();
	}

	set<AttributeSet> constructPrepatorySchema(AttributeSet startAttributes, set<FunctionalDependency> startFDs) {
		set<FunctionalDependency> singletonFDs = bernstein::decomposeFd(startFDs);
		set<FunctionalDependency> nonRedundantAttributesFDs = bernstein::removeRedundantAttributes(singletonFDs);
		//set<FunctinonalDependency> minimalFDs = removeRedundantFDs(nonRedundantAttributesFDs);
		//unordered_map<AttributeSet, set<FunctionalDependency>> partitionedFDs = bernstein::partitionFd(minimalFDs);
		//Construct relations

		throw exception();
	}

	AttributeSet superfluousAttributeDetection(set<AttributeSet>, AttributeSet) {
		throw exception();
	}
}