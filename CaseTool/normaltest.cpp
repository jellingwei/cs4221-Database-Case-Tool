// call rootProcess with parameters set<FunctionDependency> and AttributeSet 
// rootProcess returns with char type '2', '3', 'e' or 'b' for result of normal form tests
// returns 0 if either of the parameters are NULL


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
	set<int> prime;
	int sizeOfCandi;

	void resetState() {
		superKeys.clear();
		powerSet.clear();
		candidateKeys.clear();
		nonPrime.clear();
		prime.clear();
		sizeOfCandi = 0;

	}

	set<AttributeSet> getCandidateKeys() {
		return candidateKeys;
	}

	int findFactorial(int num){
		int result = num;
		for(int i=1; i<num; i++){
			result *= i;
		}
		return result;
	}
	
	char rootProcess(set<FunctionalDependency> relation, AttributeSet attr){
		//if empty return 0
		if(attr.getAttributes().empty()){
			return 0;
		}
		if(relation.empty()){
			return 'b';
		}
		
		//find set of candidate keys
		//run tests
		set<int> attributes = attr.getAttributes();
		int numberOfCandidateKeys = findCandidateKeys(relation, attr);
		nonPrime = findNonPrime(attributes);
		prime = findPrime();
		if(nonPrime.empty()){
			qDebug()<<("no non primes");
		}
		char normalForm = runNormalTests(relation, attr);
		qDebug() << QString("char is ") << normalForm;
		return normalForm;
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
			//qDebug()<< QString("working on this testset: ");
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
					//qDebug()<< QString("temp's elements are: ");
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
				//qDebug() << QString("reset");
			}
			
			//qDebug() << QString("attr size is:")<<attrSize;
			//qDebug() << QString("the end is ") << theEnd;
		}
		//qDebug() << QString("reached end of powerset part");
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
		sizeOfCandi = minimumSize;
		return candidateKeys.size();
	}

	char runNormalTests(set<FunctionalDependency> relation, AttributeSet attr){

		if(secondNormalForm(relation, attr) == '2'){
			if(thirdNormalForm(relation, attr) == '3'){
				if(elementaryKeyNormalForm(relation, attr) == 'e'){
					if(boyceCoddNormalForm(relation, attr) == 'b'){
						return 'b';
					}
					else
						return 'e';
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
		int snfCondition = 1;
		if(nonPrime.empty()){
			snfCondition = 1;
		}
		else{
			for(auto it = relation.begin(); it!=relation.end(); it++){
				FunctionalDependency fd = *it;
				set<int>rhs = fd.getRhs();
				set<int>lhs = fd.getLhs();
				AttributeSet lhsAS(lhs);
				
				for(auto it2 = rhs.begin(); it2!=rhs.end(); it2++){
					int singleAtt = *it2;
					auto nonPrimeCheck = nonPrime.find(singleAtt);
					
					//see if fd is trivial
					if(isSubsetOf(lhs, rhs)){
						snfCondition = 1;
						break;
					}
					//find if nonprime is fully dependent on candidatek eys
					else if(nonPrimeCheck != nonPrime.end()){
						auto candiCheck = candidateKeys.find(lhsAS);
						//qDebug() << QString("non prime found");
						//check if LHS is the whole key
						if(candiCheck != candidateKeys.end()){
							qDebug() << QString("non prime dependent on whole key");
							snfCondition = 1;		
						}
						else{
							//check if lhs is non prime
							// if non prime then okay
							//check if lhs is part of key
							if(lhs.size() >= sizeOfCandi){
								snfCondition = 1;
								qDebug() << QString("lhs is bigger than key");
							}
							else{
								for(auto itera=candidateKeys.begin(); itera!=candidateKeys.end(); ++itera){
									set<int> dummy;
									dummy.insert(99);
									AttributeSet iteraHolder(dummy);
									iteraHolder = *itera;

									set<int> candiSet = iteraHolder.getAttributes(); 
									//below is the case for non prime dependent on part of key
									if(isSubsetOf(candiSet, lhs)){
										snfCondition = 0;
										break;
									}
								}
								if(snfCondition == 0){
									break;
								}
							}
						}
					
					}
				if(snfCondition == 0){
					break;
				}
			}
		}
		}
		if(snfCondition)
			return '2';
		else
			return '1';
	}

	char thirdNormalForm(set<FunctionalDependency> relation, AttributeSet attr){
		int tnfCondition = 1;

		if(nonPrime.empty()){
			tnfCondition = 1;
		}
		else{
			for(auto it=relation.begin(); it!= relation.end(); ++it){
				FunctionalDependency fd = *it;
				set<int>lhs = fd.getLhs();
				set<int>rhs = fd.getRhs();
				set<int>superKeySet;
				int innerCounter = 0;

				//see if fd is trivial
				if(isSubsetOf(lhs, rhs)){
					tnfCondition = 1;
				}
				//find if rhs is prime attribute
				else if(isSubsetOf(prime, rhs)){
					qDebug() << QString("rhs is prime");
					tnfCondition=1;
				}
				else{
					for(auto it2=superKeys.begin(); it2!=superKeys.end(); ++it2){
						set<int> dummy;
						dummy.insert(99);
						AttributeSet superKey(dummy);
						superKey = *it2;
						set<int> superKeySet = superKey.getAttributes();
						innerCounter++;
						//int checkSubset = isSubsetOf(superKeySet, lhs);
					
						//if(checkSubset){
							//break;
						//}
						if(lhs == superKeySet){
							break;
						}

						if(innerCounter == superKeys.size()){
							tnfCondition = 0;
						}
					}
				}
				if(tnfCondition == 0){
					break;
				}
			}
		}
		if(tnfCondition)
			return '3';
		else
			return '2';
	}

	char elementaryKeyNormalForm(set<FunctionalDependency> relation, AttributeSet attr){
		int eknfCondition = 1;
		int eknfLoopCounter = 0;
		for(auto it=relation.begin(); it != relation.end(); ++it){
			FunctionalDependency fd = *it;
			set<int> lhs = fd.getLhs();
			set<int> rhs = fd.getRhs();
			int it2LoopChecker = 1;
			eknfLoopCounter++;
			if(eknfLoopCounter == relation.size()){
				break;
			}
			//---------------------check if trivial
			if(isSubsetOf(lhs, rhs)){
				eknfCondition = 1;
			}
			else{
				for(auto it2 = relation.begin(); it2 !=relation.end(); ++it2){
					//--------------------------start comparison from next fd
					//--------------------------compare fds to check for full dependency
					if(it2LoopChecker == 1){
						it2 = it;
						++it2;
						it2LoopChecker = 0;
						////test
						//FunctionalDependency fdTest = *it2;
						//set<int>lhsTest = fdTest.getLhs();
						//FunctionalDependency fdTest2 = *it;
						//set<int>lhsTest2 = fdTest2.getLhs();
						////qDebug() << QString("THIS IS THE FD YOU WANT TO SEE");
						//for(auto testIT = lhsTest.begin(); testIT != lhsTest.end(); ++testIT){
						//	qDebug() << *testIT;
						//}
						//qDebug() << QString("THIS IS THE FD YOU comparing with");
						//for(auto testIT2 = lhsTest2.begin(); testIT2 != lhsTest2.end(); ++testIT2){
						//	qDebug() << *testIT2;
						//}
					}
					FunctionalDependency fd2 = *it2;
					set<int>rhs2 = fd2.getRhs();
					set<int>lhs2 = fd2.getLhs();
					////test
					//qDebug() << QString("1st rhs");
					//for(auto iter1 = rhs.begin(); iter1!=rhs.end(); ++iter1){
					//	qDebug() << *iter1;
					//}
					//qDebug() << QString("2nd rhs");
					//for(auto iter2 = rhs2.begin(); iter2!=rhs2.end(); ++iter2){
					//	qDebug() << *iter2;
					//}
					if(rhs2 == rhs){
						if(isSubsetOf(lhs, lhs2) || isSubsetOf(lhs2, lhs)){
							//problem may arise if im comparing the same fds
							//qDebug() << QString("not eknf you gg");
							eknfCondition = 0;
							break;
						}
					}
				}	
			}
			if(eknfCondition == 0){
				break;
			}

		}
		if(eknfCondition){
			return 'e';
		}
		else
			return '3';
	}

	char boyceCoddNormalForm(set<FunctionalDependency> relation, AttributeSet attr){
		int bcnfCondition = 1;
		for(auto it=relation.begin(); it != relation.end(); ++it){
			FunctionalDependency fd = *it;
			set<int> lhs = fd.getLhs();
			set<int> rhs = fd.getRhs();
			
			//see if fd is trivial
			if(isSubsetOf(lhs, rhs)){
				bcnfCondition = 1;
				break;
			}
			int innerLoopCounter = 0;
			for(auto it2=superKeys.begin(); it2 != superKeys.end(); ++it2){
				innerLoopCounter++;
				set<int> dummy;
				dummy.insert(99);
				AttributeSet singleSuperKey(dummy);
				singleSuperKey = *it2;
				set<int> singleSuperKeySet = singleSuperKey.getAttributes();
				//see if lhs is superkey
				if(lhs == singleSuperKeySet){
					innerLoopCounter = 0;
					break;
				}
			}
			if(innerLoopCounter == superKeys.size()){
				bcnfCondition = 0;
				break;
			}
		}
		if(bcnfCondition)
			return 'b';
		else
			return 'e';
	}
	
	set<int> findPrime(){
		set<int> primeSet;
		for(auto it=candidateKeys.begin(); it!=candidateKeys.end(); ++it){
			set<int> dummy;
			dummy.insert(99);
			AttributeSet candiAS(dummy);
			candiAS = *it;
			set<int> candiSet = candiAS.getAttributes();
			for(auto it2=candiSet.begin(); it2!=candiSet.end(); ++it2){
				primeSet.insert(*it2);
			}
		}
		return primeSet;
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

	//returns if sub is a subset of super
	int isSubsetOf(set<int> super, set<int> sub){
		int size = sub.size();
		int count = 0;
		for(auto it=sub.begin(); it!= sub.end(); ++it){
			for(auto it2= super.begin(); it2!= super.end(); it2++){
				if(*it2 == *it){
					count++;
				}
			}
		}

		if(size == count){
			//qDebug() << QString("yes subset");
			return 1; 
		}
		else{
			//qDebug() << QString("not subset");
			return 0;
		}
	}
}