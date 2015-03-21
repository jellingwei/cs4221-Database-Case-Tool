#include <set>
#include <unordered_map>

#include <qDebug>

#include "casetool.h"
#include "functionaldependency.h"
#include "bernstein.h"
#include "AttributeSet.h"



using std::vector;
using std::set;
using std::string;
using std::unordered_map;

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

	if(!lhs.empty() && !rhs.empty()) {
		showFD(fd);
	} else {
		qDebug() << "lhs or rhs is empty";
	}

	lhs.clear();
	rhs.clear();

	ui.lhsAttr1->setChecked(false);
	ui.lhsAttr2->setChecked(false);
	ui.lhsAttr3->setChecked(false);
	ui.lhsAttr4->setChecked(false);
	ui.lhsAttr5->setChecked(false);
	ui.lhsAttr6->setChecked(false);
	ui.rhsAttr1->setChecked(false);
	ui.rhsAttr2->setChecked(false);
	ui.rhsAttr3->setChecked(false);
	ui.rhsAttr4->setChecked(false);
	ui.rhsAttr5->setChecked(false);
	ui.rhsAttr6->setChecked(false);

	return;
}

void CaseTool::showFD(FunctionalDependency fd) {
	QListWidgetItem *item = new QListWidgetItem(QString(fd.display().c_str()), ui.FDList);
	
	ui.FDList->setCurrentItem(item);

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


	if (isChecked && name == "lhsAttr1") {
		lhs.insert(0);
		qDebug() << "A!";
	} else if (isChecked && name == "lhsAttr2") {
		lhs.insert(1);
		qDebug() << "B!";
	} else if (isChecked && name == "lhsAttr3") {
		lhs.insert(2);
		qDebug() << "C!";
	} else if (isChecked && name == "lhsAttr4") {
		lhs.insert(3);
		qDebug() << "D!";
	} else if (isChecked && name == "lhsAttr5") {
		lhs.insert(4);
		qDebug() << "E!";
	} else if (isChecked && name == "lhsAttr6") {
		lhs.insert(5);
		qDebug() << "F!";
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

	if (isChecked && name == "rhsAttr1") {
		rhs.insert(0);
		qDebug() << "A!";
	} else if (isChecked && name == "rhsAttr2") {
		rhs.insert(1);
		qDebug() << "B!";
	} else if (isChecked && name == "rhsAttr3") {
		rhs.insert(2);
		qDebug() << "C!";
	} else if (isChecked && name == "rhsAttr4") {
		rhs.insert(3);
		qDebug() << "D!";
	} else if (isChecked && name == "rhsAttr5") {
		rhs.insert(4);
		qDebug() << "E!";
	} else if (isChecked && name == "rhsAttr6") {
		rhs.insert(5);
		qDebug() << "F!";
	}
}

void CaseTool::numOfAttributes() {

}


void CaseTool::runBernstein() {

	ui.outputList->clear();

	set<FunctionalDependency> fdSet = functionalDependecies;

	// step 1
	string step1Separator = "Step 1:";
	QListWidgetItem *item = new QListWidgetItem(QString(step1Separator.c_str()), ui.outputList);
	item->setData(Qt::UserRole, QString(step1Separator.c_str()));

	ui.outputList->setCurrentItem(item);
	set<FunctionalDependency> newFdSet = bernstein::removeRedundantAttributes(fdSet);

	for (auto iter = newFdSet.begin(); iter != newFdSet.end(); ++iter) {
		FunctionalDependency fd = *iter;
		string fdStr = fd.display();
		QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(fdStr.c_str()));

		ui.outputList->setCurrentItem(item);
	}
	
	// step 2...
	string step2Separator = "Step 2:";
	QListWidgetItem *stepWidget2 = new QListWidgetItem(QString(step2Separator.c_str()), ui.outputList);
	//item->setData(Qt::UserRole, QString(step2Separator.c_str()));

	ui.outputList->setCurrentItem(stepWidget2);
	set<FunctionalDependency> minimalCover = bernstein::obtainMinimalCover(fdSet);
	for (auto iter = minimalCover.begin(); iter != minimalCover.end(); ++iter) {
		FunctionalDependency fd = *iter;
		string fdStr = fd.display();
		QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(fdStr.c_str()));

		ui.outputList->setCurrentItem(item);
	}

	// step 3
	string step3Separator = "Step 3:";
	QListWidgetItem *stepWidget3 = new QListWidgetItem(QString(step3Separator.c_str()), ui.outputList);
	ui.outputList->setCurrentItem(stepWidget3);

	unordered_map<AttributeSet, set<FunctionalDependency> > partitions = bernstein::partitionFd(minimalCover);
	for (auto iter = partitions.begin(); iter != partitions.end(); ++iter) {
		set<FunctionalDependency> fdSet = iter->second;

		for (auto fdIter = fdSet.begin(); fdIter != fdSet.end(); ++fdIter) {
			FunctionalDependency fd = *fdIter;
			string fdStr = fd.display();
			QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
			item->setData(Qt::UserRole, QString(fdStr.c_str()));

			ui.outputList->setCurrentItem(item);
		}
	}


	// step 4
	string step4Separator = "Step 4:";
	QListWidgetItem *stepWidget4 = new QListWidgetItem(QString(step4Separator.c_str()), ui.outputList);
	ui.outputList->setCurrentItem(stepWidget4);

	partitions = bernstein::mergeEquivalentKeys(partitions, minimalCover);
	for (auto iter = partitions.begin(); iter != partitions.end(); ++iter) {
		set<FunctionalDependency> fdSet = iter->second;

		for (auto fdIter = fdSet.begin(); fdIter != fdSet.end(); ++fdIter) {
			FunctionalDependency fd = *fdIter;
			string fdStr = fd.display();
			QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
			item->setData(Qt::UserRole, QString(fdStr.c_str()));

			ui.outputList->setCurrentItem(item);
		}
	}

	// step 5
	string step5Separator = "Step 5:";
	QListWidgetItem *stepWidget5 = new QListWidgetItem(QString(step5Separator.c_str()), ui.outputList);
	ui.outputList->setCurrentItem(stepWidget5);

	set<int> emptySet;

	set<FunctionalDependency> allFdAfterPartitioning = bernstein::createSetOfFDFromPartitions(partitions);
	partitions = bernstein::eliminateTransitiveDependenciesForPartition(partitions, allFdAfterPartitioning);
	for (auto iter = partitions.begin(); iter != partitions.end(); ++iter) {
		set<FunctionalDependency> fdSet = iter->second;

		for (auto fdIter = fdSet.begin(); fdIter != fdSet.end(); ++fdIter) {
			FunctionalDependency fd = *fdIter;
			string fdStr = fd.display();
			QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
			item->setData(Qt::UserRole, QString(fdStr.c_str()));

			ui.outputList->setCurrentItem(item);
		}
	}

	// step 6
	string step6Separator = "Step 6:";
	item = new QListWidgetItem(QString(step6Separator.c_str()), ui.outputList);
	item->setData(Qt::UserRole, QString(step6Separator.c_str()));

	ui.outputList->setCurrentItem(item);
	set<std::pair<AttributeSet, set<AttributeSet> > > finalAnswer = bernstein::constructRelations(partitions);

	for (auto iter = finalAnswer.begin(); iter != finalAnswer.end(); ++iter) {
		AttributeSet attrsInRelation = iter->first;
		set<AttributeSet> keys = iter->second;
		
		set<int> attrs = attrsInRelation.getAttributes();
		string attrsStr;
		for (auto iter = attrs.begin(); iter != attrs.end(); ++iter) {
			attrsStr += std::to_string(static_cast<long long>(*iter));
		}

		QListWidgetItem *item = new QListWidgetItem(QString(attrsStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(attrsStr.c_str()));

		ui.outputList->setCurrentItem(item);
	}

}

