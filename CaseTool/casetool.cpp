#include <set>
#include <qDebug>

#include "casetool.h"
#include "functionaldependency.h"


using std::vector;
using std::set;

CaseTool::CaseTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


}

CaseTool::~CaseTool()
{

}


set<int> lhs;
set<int> rhs;

set<FunctionalDependency> functionalDependecies;

void CaseTool::addFD() {
	FunctionalDependency fd(lhs, rhs);

	functionalDependecies.insert(fd);

	lhs.clear();
	rhs.clear();

	return;
}

void CaseTool::addLhsToFd(bool isChecked){
	QObject* obj = sender();
	QCheckBox* emittingCheckbox = dynamic_cast<QCheckBox*>(obj);

	QString name = emittingCheckbox->objectName();

	if (isChecked) {
		qDebug() << "Checked LHS!";
	} else{
		qDebug() << "Unchecked LHS!";
	}


	if (name == "lhsAttr1") {
		lhs.insert(0);
		qDebug() << "A!";
	} else if (name == "lhsAttr2") {
		lhs.insert(1);
		qDebug() << "B!";
	} else if (name == "lhsAttr3") {
		lhs.insert(2);
		qDebug() << "C!";
	}
}

void CaseTool::addRhsToFd(bool isChecked){
	QObject* obj = sender();
	QCheckBox* emittingCheckbox = dynamic_cast<QCheckBox*>(obj);

	QString name = emittingCheckbox->objectName();

	if (isChecked) {
		qDebug() << "Checked RHS!";
	} else{
		qDebug() << "Unchecked RHS!";
	}


	if (name == "rhsAttr1") {
		rhs.insert(0);
		qDebug() << "A!";
	} else if (name == "rhsAttr2") {
		rhs.insert(1);
		qDebug() << "B!";
	} else if (name == "rhsAttr3") {
		rhs.insert(2);
		qDebug() << "C!";
	}

}

void CaseTool::numOfAttributes(int num) {

}
