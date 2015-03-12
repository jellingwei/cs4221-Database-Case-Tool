
#include <string.h>
#include "normaltest.h"
#include <QtWidgets/QApplication>
#include <QDebug> 
#include <stack>

namespace normalTest {
	set<AttributeSet> superKeys;
	set<AttributeSet> powerSet;
	set<AttributeSet> candidateKeys;
	set<int> nonPrime;

	int findFactorial(int num){
		int result = num;
		for(int i=1; i<num; i++){
			result *= i;
		}
		return result;
	}
	
	int rootFoo(set<FunctionalDependency> relation, AttributeSet attr){
		//find set of candidate keys
		//run tests
		set<int> attributes = attr.getAttributes();
		int numberOfCandidateKeys = findCandidateKeys(relation, attr);
		nonPrime = findNonPrime(attributes);
		char normalForm = runNormalTests(relation, attr);
		return numberOfCandidateKeys;
	}
	

	int findCandidateKeys(set<FunctionalDependency> relation, AttributeSet attr){
		int attrSize = attr.size();
		int theEnd = 0; // the expected number of sets in powerSet per iteration
		//qDebug() << QString("Okay entering loop");

		//--------- start of finding power set----------------------------------
		powerSet.insert(attr); theEnd++;
		int numberOfFirstIter = attr.getAttributes().size();
		theEnd+=numberOfFirstIter;
		//find first iteration
		int loopIter = 0;
		//===========================================
		while(numberOfFirstIter > 0){
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
			numberOfFirstIter--; 
			//============================================================
		}
		//qDebug() << QString("first loop results");
		//test function ----------------------------------------------------------------------
		//int mySize;
		//for(auto it = powerSet.begin(); it != powerSet.end(); ++it){
		//	set<int> dummy;
		//	dummy.insert(99);
		//	AttributeSet tempAttributeSet(dummy);
		//	
		//	tempAttributeSet = *it;

		//	set<int> testSet = tempAttributeSet.getAttributes(); //get individual set

		//	for(auto it = testSet.begin(); it != testSet.end(); it++){
		//		qDebug()<< *it;
		//	}
		//	mySize = powerSet.size();
		//	qDebug()<< endl;
		//}
		//qDebug() << QString("my size is ") << mySize;
		///test function ----------------------------------------------------------------
		int performFactorialCheck = 1;
		int conAttrSize = attrSize;

		for(auto it = powerSet.begin(); attrSize!= 0; ++it){
			
			if(performFactorialCheck){
				int numerator = findFactorial(conAttrSize);
				//qDebug() << numerator;
				int cNumber;
				if(attrSize-2>0){
					cNumber = attrSize-2;
					int intermediate = conAttrSize-cNumber;
					intermediate = findFactorial(intermediate);
					cNumber = findFactorial(cNumber);
					cNumber *= intermediate;
					//qDebug() << cNumber;
					theEnd += (numerator/cNumber);
					//find 4cX here

				}
				performFactorialCheck = 0;

			}
			//start from 2nd iteration
			//first to get one individual attribute set
			//qDebug()<< QString("start of iteration!");
			set<int> dummy;
			dummy.insert(99);
			AttributeSet tempAttributeSet(dummy);
			
			tempAttributeSet = *it;
			//qDebug()<< QString("before assigning tempset to it");
			set<int> testSet = tempAttributeSet.getAttributes(); //get individual set
			qDebug()<< QString("working on this testset: ");
			for(auto it2 = testSet.begin(); it2 != testSet.end(); ++it2){
				qDebug() << *it2;
			}
								
			//split into smaller subsets
			//==========================================================================
			if(attrSize - testSet.size() == 1 && testSet.size()>1){
				int loopIterator = 0;
				while(loopIterator<testSet.size()){
					set<int> temp = testSet;
					auto iter = temp.begin();

					//qDebug() << QString("temp size is:")<<temp.size();

					int k = loopIterator;
					//iterate to the correct element
					while(1){
						if(k==0)
							break;
						++iter;
						k--;
					}

					temp.erase(iter);
					qDebug()<< QString("temp's elements are: ");
					for(auto it3 = temp.begin(); it3 != temp.end(); ++it3){
						qDebug()<< *it3;
					}
					powerSet.insert(temp);
					loopIterator++;
				}
				
				
			}
			else
				qDebug() << QString("This set did not enter iterative part");
			auto lastSet = powerSet.end();

			// if it equals to lastSet does not work
			if(powerSet.size() == theEnd){
				attrSize--; // can only decrease after all is done
				it = powerSet.begin();
				performFactorialCheck =1;
				qDebug() << QString("reset");
			}
			
			//qDebug() << QString("attr size is:")<<attrSize;
			//qDebug() << QString("the end is ") << theEnd;
		}
		qDebug() << QString("reached end of powerset part");
		qDebug() << QString("These are what is in the powerset in order");
		for(auto itera=powerSet.begin(); itera!= powerSet.end(); ++itera){
			set<int> testSett;
			testSett.insert(99);
			AttributeSet testAS(testSett);
			testAS = *itera;
			set<int> testSett2 = testAS.getAttributes();
			qDebug() << QString("{");
			for(auto itera2=testSett2.begin(); itera2 != testSett2.end(); ++itera2){
				qDebug() << *itera2;
			}
			qDebug() << QString("}");
		}
	// ---------------end of finding powerset----------------

		//--------find candidate key-----------------
		
		for(auto it = powerSet.begin(); it!= powerSet.end(); ++it){
			//insert dummy to initialise attributeset
			set<int> dummy2;
			dummy2.insert(99);
			AttributeSet dummyAS(dummy2);
			dummyAS = *it; //not sure if erase or append
			set<int> individualSet = dummyAS.getAttributes();
			//set<int>::iterator dummyIter;
			//dummyIter = individualSet.find(99);
			//individualSet.erase(dummyIter);
			//testing each element in powerset per iteration to see if they are candidate keys
			AttributeSet potentialCandidateKey(individualSet);
			if(potentialCandidateKey.getAttributeClosure(relation).getAttributes() == attr.getAttributeClosure(relation).getAttributes()){
				superKeys.insert(potentialCandidateKey);
			}
		
		}
		//find minimal size
		// print candidate
		int minimumSize = 99;
		qDebug() << QString("Printing superkeys:");
		for(auto it=superKeys.begin(); it!=superKeys.end(); ++it){
			set<int> dumbo;
			dumbo.insert(99);
			AttributeSet candySet(dumbo);
			candySet = *it;
			
			set<int> candySet_set;
			candySet_set = candySet.getAttributes();
			if(candySet_set.size() < minimumSize){
				minimumSize = candySet_set.size();
			}
			qDebug() << QString("{");
			for(auto iter=candySet_set.begin(); iter!=candySet_set.end(); ++iter){
				qDebug() << *iter;
			}
			qDebug() << QString("}");
		}
		for(auto it=superKeys.begin(); it!=superKeys.end(); ++it){
			set<int> dumbo;
			dumbo.insert(99);
			AttributeSet candySet(dumbo);
			candySet = *it;
			
			set<int> candySet_set;
			candySet_set = candySet.getAttributes();
			if(candySet_set.size() == minimumSize){
				candidateKeys.insert(candySet);
				qDebug()<< QString("candidate key inserted!");
			}
		}
		return candidateKeys.size();
	}


	char runNormalTests(set<FunctionalDependency> relation, AttributeSet attr){

		if(secondNormalForm(relation, attr) == '2'){
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
	char secondNormalForm(set<FunctionalDependency> relation, AttributeSet attr){
		for(auto it = relation.begin(); it!=relation.end(); it++){
			FunctionalDependency fd = *it;
			
		}
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

	set<int> findNonPrime(set<int> attributes){
		//attributes to hold all attributes
		std::stack<int> holdAll;
		std::stack<int> remains;
		std::stack<int> empty;
		set<int> nonPrime;
		for(auto it=attributes.begin(); it!=attributes.end(); ++it){
			holdAll.push(*it);
		}

		for(auto it=candidateKeys.begin(); it!=candidateKeys.end(); ++it){
			
			set<int> dummy;
			dummy.insert(99);
			AttributeSet dummyAS(dummy);
			dummyAS = *it;
			set<int> candidateSet = dummyAS.getAttributes();
			int sizeOfStack = holdAll.size();
			for(int i =0; i<sizeOfStack; i++){
				int topNum = holdAll.top();	
				auto iter = candidateSet.find(topNum);
				if(iter == candidateSet.end()){
					remains.push(topNum);
				}
				holdAll.pop();
			}
			holdAll = remains;
			remains = empty;
			//rebuild non prime set
		}
		int sizeOfRemains = holdAll.size();
		for(int i=0; i<sizeOfRemains; i++){
			int nonPrimeA = holdAll.top();
			qDebug() << QString("Non prime here:") << nonPrimeA;
			nonPrime.insert(nonPrimeA);
			holdAll.pop();
		}
		return nonPrime;
	}

}