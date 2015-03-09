#include "casetool.h"
#include <QtWidgets/QApplication>
#include <QDebug> 

#include <iostream>
#include "attributeset.h"
#include "bernstein.h"

using std::string;

void testtest() {
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

	set<FunctionalDependency> fdSet;
	fdSet.insert(fd); fdSet.insert(fd1);

	set<FunctionalDependency> newFdSet = bernstein::removeRedundantAttributes(fdSet);
	for (auto iter = newFdSet.begin(); iter != newFdSet.end(); ++iter) {
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

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CaseTool w;
	testtest();
	w.show();

	return a.exec();
}
