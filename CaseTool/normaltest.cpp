#include "normaltest.h"
#include <string.h>


namespace normalTest {

	void rootFoo(set<FunctionalDependency> relation, AttributeSet attr){
		//find set of candidate keys
		//run tests
		int numberOfCandidateKeys = findCandidateKeys(relation, attr);
		char normalForm = runNormalTests(relation);
	}
	


	int findCandidateKeys(set<FunctionalDependency> relation, AttributeSet attr){
		int attrSize = attr.size();

		//--------- start of finding power set----------------------------------
		powerSet.insert(attr);
		int numberOfFirstIter = attr.getAttributes().size();
		//find first iteration
		int loopIter = 0;
		while(numberOfFirstIter>0){
			set<int> temp = attr.getAttributes();
			auto iter = temp.begin();

			int k = loopIter;
			while(1){
				if(k==0)
					break;
				++iter;
				k--;
			}

			temp.erase(iter);
			powerSet.insert(temp);
			loopIter++;
		}

		for(auto it = powerSet.begin(); it != powerSet.end(); ++it){
			//start from 2nd iteration
			//first to get one individual attribute set
			set<int> dummy;
			dummy.insert(99);
			AttributeSet tempAttributeSet(dummy);
			tempAttributeSet = *it;
			set<int> testSet = tempAttributeSet.getAttributes(); //get individual set
			// find dummy and erase it
			set<int>::iterator dummyIter;
			dummyIter = testSet.find(99);
			testSet.erase(dummyIter);
			//split into smaller subsets
			if(testSet.size() < attrSize){
				int loopIterator = testSet.size();
				while(loopIterator>0){
					set<int> temp = testSet;
					auto iter = temp.begin();

					int k = loopIterator;
					//iterate to the correct element
					while(1){
						if(k==3)
							break;
						++iter;
						k++;
					}

					temp.erase(iter);
					powerSet.insert(temp);
					loopIterator--;
				}
			}
			attrSize--;
		};
		// ---------------end of finding powerset----------------

		//--------find candidate key-----------------
		
		for(auto it = powerSet.begin(); it!= powerSet.end(); ++it){
			//insert dummy to initialise attributeset
			set<int> dummy2;
			dummy2.insert(99);
			AttributeSet dummyAS(dummy2);
			dummyAS = *it; //not sure if erase or append
			set<int> individualSet = dummyAS.getAttributes();
			set<int>::iterator dummyIter;
			dummyIter = individualSet.find(99);
			individualSet.erase(dummyIter);
			//testing each element in powerset per iteration to see if they are candidate keys
			AttributeSet potentialCandidateKey(individualSet);
			if(potentialCandidateKey.getAttributeClosure(relation).getAttributes() == attr.getAttributeClosure(relation).getAttributes())
				candidateKeys.insert(potentialCandidateKey);
		}
		return candidateKeys.size();
	}


	char runNormalTests(set<FunctionalDependency> relation){

		if(secondNormalForm(relation) == '2'){
			if(thirdNormalForm(relation) == '3'){
				if(boyceCoddNormalForm(relation) == 'b'){
					if(elementaryKeyNormalForm(relation) == 'e'){
						return 'e';
					}
					else
						return 'b';
				}
				else
					return '3';
			}
			else
				return '2';
		}
		return '1';
	}
	char secondNormalForm(set<FunctionalDependency> relation){
		if(true)
			return '2';
		else
			return '1';
	}
	char thirdNormalForm(set<FunctionalDependency> relation){
		if(true)
			return '3';
		else
			return '2';
	}
	char boyceCoddNormalForm(set<FunctionalDependency> relation){
		if(true)
			return 'b';
		else
			return '3';
	}
	char elementaryKeyNormalForm(set<FunctionalDependency> relation){
		if(true)
			return 'e';
		else
			return 'b';
	}

}