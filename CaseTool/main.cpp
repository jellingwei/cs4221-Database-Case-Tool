#include <QtWidgets/QApplication>
#include <QDebug> 

#include <iostream>
#include <unordered_map>

#include "casetool.h"
#include "attributeset.h"
#include "bernstein.h"

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

	FunctionalDependency fd(fdLhs, fdRhs);

	set<int> fdLhs1;
	fdLhs1.insert(0);

	set<int> fdRhs1;
	fdRhs1.insert(1);
	fdRhs1.insert(2);

	FunctionalDependency fd1(fdLhs1, fdRhs1);

	set<int> fdLhs2;
	fdLhs2.insert(3);

	set<int> fdRhs2;
	fdRhs2.insert(4);

	FunctionalDependency fd2(fdLhs2, fdRhs2);

	set<int> fdLhs3;
	fdLhs3.insert(0);
	fdLhs3.insert(1);

	set<int> fdRhs3;
	fdRhs3.insert(4);

	FunctionalDependency fd3(fdLhs3, fdRhs3);

	set<int> fdLhs4;
	fdLhs4.insert(3);

	set<int> fdRhs4;
	fdRhs4.insert(0);

	FunctionalDependency fd4(fdLhs4, fdRhs4);

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

			qDebug() << QString(lhsStr.c_str()) << "->" << QString(rhsStr.c_str()); 
		}
	}
	

}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CaseTool w;
	seeOutputSpecificCase();
	w.show();

	return a.exec();
}
