#include <exception>
#include <unordered_map>

#include "ltk.h"
#include "bernstein.h"

namespace ltk {
	using std::set;
	using std::exception;
	using std::unordered_map;

	set<AttributeSet> constructPreparatorySchema(AttributeSet startAttributes, set<FunctionalDependency> startFDs) {
		set<FunctionalDependency> singletonFDs = bernstein::decomposeFd(startFDs);
		set<FunctionalDependency> nonRedundantAttributesFDs = bernstein::removeRedundantAttributes(singletonFDs);
		//set<FunctinonalDependency> minimalFDs = removeRedundantFDs(nonRedundantAttributesFDs);
		//unordered_map<AttributeSet, set<FunctionalDependency>> partitionedFDs = bernstein::partitionFd(minimalFDs);
		//Construct relations
		//Augment preparatory schema(constructed relations, minimalFDs);
		throw exception();
	}

	set<AttributeSet> augmentPreparatorySchema(set<AttributeSet> constructedRelations, set<FunctionalDependency> minimalFDs) {
		//For each constructed relation, check if the closure of attributes can give back the whole initial set of attributes
		//If cannot, construct a subset of the initial set of attributes such that its closure can give it back
		//Add this relation to the preparatory schema to give the whole preparatory schema
		throw exception();
	}

	//Outputs the new set of synthesized key of relation if attribute is superfluous
	//Otherwise outputs null
	AttributeSet superfluousAttributeDetection(set<AttributeSet> preparatoryRelation, AttributeSet singleAttribute) {
		//Mark attribute as superfluous
		throw exception();
	}

	set<AttributeSet> deletionNormalization(AttributeSet attributes, set<FunctionalDependency> FDs) {
		//Construct preparatory schema from above
		//For each relation, for each attribute, 
		//if superfluous attribute detection returns a nonempty set, construct a new relation and replace in preparatory schema
		//Final output is in LTK normal form
		throw exception();
	}
}