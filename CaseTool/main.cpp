#include <QtWidgets/QApplication>
#include <QDebug> 

#include <iostream>
#include <unordered_map>

#include "casetool.h"
#include "attributeset.h"
#include "bernstein.h"
#include "normaltest.h"

using std::string;
using std::unordered_map;

void seeOutputSpecificCase() {
	//@todo put in unit test...
	set<int> fdLhs;
	fdLhs.insert(0);
	fdLhs.insert(1);
	fdLhs.insert(2);

	set<int> fdRhs;
	fdRhs.insert(3);

	FunctionalDependency fd(fdLhs, fdRhs);				// 012 -> 3

	set<int> fdLhs1;
	fdLhs1.insert(0);

	set<int> fdRhs1;
	fdRhs1.insert(1);
	fdRhs1.insert(2);

	FunctionalDependency fd1(fdLhs1, fdRhs1);			// 0 -> 12

	set<int> fdLhs2;
	fdLhs2.insert(3);

	set<int> fdRhs2;
	fdRhs2.insert(4);

	FunctionalDependency fd2(fdLhs2, fdRhs2);			// 3 ->4

	set<int> fdLhs3;
	fdLhs3.insert(0);
	fdLhs3.insert(1);

	set<int> fdRhs3;
	fdRhs3.insert(4);

	FunctionalDependency fd3(fdLhs3, fdRhs3);			//01 -> 4

	set<int> fdLhs4;
	fdLhs4.insert(3);

	set<int> fdRhs4;
	fdRhs4.insert(0);

	FunctionalDependency fd4(fdLhs4, fdRhs4);			//3->0

	set<FunctionalDependency> fdSet;
	fdSet.insert(fd); fdSet.insert(fd1);
	fdSet.insert(fd2); fdSet.insert(fd3);
	fdSet.insert(fd4);

	set<FunctionalDependency> newFdSet = bernstein::removeRedundantAttributes(fdSet);
	
	// step 2?
	set<FunctionalDependency> minimalCover = bernstein::obtainMinimalCover(newFdSet);

	// step 3
	unordered_map<AttributeSet, set<FunctionalDependency> > partitions = bernstein::partitionFd(minimalCover);

	// step 4?
	partitions = bernstein::mergeEquivalentKeys(partitions, minimalCover);

	for (auto partitionIter = partitions.begin(); partitionIter != partitions.end(); ++partitionIter) {
		set<FunctionalDependency> fdInPartition = partitionIter->second;

		qDebug() << QString("-------");
		for (auto iter = fdInPartition.begin(); iter != fdInPartition.end(); ++iter) {
			FunctionalDependency fd = *iter;
			set<int> lhs = fd.getLhs();
			set<int> rhs = fd.getRhs();
			string lhsStr;
			for (auto iter = lhs.begin(); iter != lhs.end(); ++iter) {
				lhsStr += std::to_string(static_cast<long long>(*iter));
			}
			string rhsStr;
			for (auto iter = rhs.begin(); iter != rhs.end(); ++iter) {
				rhsStr += std::to_string(static_cast<long long>(*iter));
			}

			qDebug() << QString(fd.display().c_str());
		}
		
	}

	// @todo: skip step 5 for now. 


	// step 6
	qDebug() << QString("---final----");
	set<std::pair<AttributeSet, set<AttributeSet> > > finalAnswer = bernstein::constructRelations(partitions);
	for (auto partitionIter = finalAnswer.begin(); partitionIter != finalAnswer.end(); ++partitionIter) {
		AttributeSet attributesInRelation = partitionIter->first;
		set<AttributeSet> keys = partitionIter->second;

		qDebug() << QString("-------");
		set<int> attrs = attributesInRelation.getAttributes();
		string attrsStr;
		for (auto iter = attrs.begin(); iter != attrs.end(); ++iter) {
			attrsStr += std::to_string(static_cast<long long>(*iter));
		}
		qDebug() << QString(attrsStr.c_str());


		qDebug() << QString("keys are:");
		for (auto iter = keys.begin(); iter != keys.end(); ++iter) {
			set<int> attrs = (*iter).getAttributes();
			string attrsStr;
			for (auto iter = attrs.begin(); iter != attrs.end(); ++iter) {
				attrsStr += std::to_string(static_cast<long long>(*iter));
			}
			qDebug() << QString(attrsStr.c_str());
		}

		
	}
	
	

}

void normaltest_test(){
	//ab->c, c->d, d->b
	
	//ab, ad, ac are keys
	qDebug() << QString("start of my test");
	set<int> lhs_1;
	set<int> rhs_1;
	lhs_1.insert(0);
	lhs_1.insert(1);
	rhs_1.insert(2);

	FunctionalDependency fd1(lhs_1, rhs_1);

	set<int> lhs_2;
	set<int> rhs_2;
	lhs_2.insert(2);
	rhs_2.insert(3);
	FunctionalDependency fd2(lhs_2, rhs_2);

	
	set<int> lhs_3;
	set<int> rhs_3;
	lhs_3.insert(3);
	rhs_3.insert(1);
	FunctionalDependency fd3(lhs_3, rhs_3);

	set<int> attr;
	attr.insert(0);
	attr.insert(1);
	attr.insert(2);
	attr.insert(3);
	AttributeSet attrSet(attr);
	
	//theproblem is below
	set<FunctionalDependency> fdSett;
	fdSett.insert(fd1);
	qDebug() << QString("fd1 inserted!");
	fdSett.insert(fd2);
	qDebug() << QString("fd2 inserted!");
	//fdSett.insert(fd3);
	//qDebug() << QString("fd3 inserted!");
	//qDebug() << QString("Still great");
	int numResult = normalTest::rootFoo(fdSett, attrSet);

	//qDebug() << QString("result is: ") << numResult;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CaseTool w;
	seeOutputSpecificCase();
	//normaltest_test();
	w.show();

	return a.exec();
}
