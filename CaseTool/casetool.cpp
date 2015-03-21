#include <set>
#include <unordered_map>
#include <sstream>

#include <QVBoxLayout>
#include <QCheckBox>
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
int numAttributes;

vector<QCheckBox*> lhsCheckBox;
vector<QCheckBox*> rhsCheckBox;


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

	for (auto iter  = lhsCheckBox.begin(); iter != lhsCheckBox.end(); ++iter) {
		(*iter)->setChecked(false);
	}

	for (auto iter  = rhsCheckBox.begin(); iter != rhsCheckBox.end(); ++iter) {
		(*iter)->setChecked(false);
	}

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

	for (int i = 0; i < numAttributes; i++) {
		string attrName = "lhsAttr";
		attrName += std::to_string(static_cast<long long>(i + 1));
		QString qstr = QString::fromStdString(attrName);

		if (isChecked && name==qstr) {
			lhs.insert(i);
			qDebug() << i;
		}
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

	for (int i = 0; i < numAttributes; i++) {
		string attrName = "rhsAttr";
		attrName += std::to_string(static_cast<long long>(i + 1));
		QString qstr = QString::fromStdString(attrName);

		if (isChecked && name==qstr) {
			rhs.insert(i);
			qDebug() << i;
		}
	}
}

void clearLayout(QLayout *layout){
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

string getTextOfCheckbox(int i) {
	char i_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i];
	std::stringstream ss;
	ss << i_char;
	string stringToDisplay;
	ss >> stringToDisplay;
	return stringToDisplay;
}

void CaseTool::numOfAttributes() {
	numAttributes = ui.numOfAttributes->value();

	if (ui.scrollArea->layout() != NULL) {
		clearLayout(ui.scrollArea->layout());
		lhsCheckBox.clear();

		delete ui.scrollArea->layout();
	}

	if (ui.scrollArea2->layout() != NULL) {
		clearLayout(ui.scrollArea2->layout());
		rhsCheckBox.clear();

		delete ui.scrollArea2->layout() ;
	}

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->setDirection(QBoxLayout::LeftToRight);
	for(int i=0; i < numAttributes; i++)
	{
		QCheckBox* dynamic = new QCheckBox("");
		string name = "lhsAttr";
		name += std::to_string(static_cast<long long>(i + 1));
		dynamic->setObjectName(QString(name.c_str()));
		qDebug() <<QString(name.c_str());
		dynamic->setChecked (false);
		dynamic->setText (QString(getTextOfCheckbox(i).c_str()));
		lay->addWidget(dynamic);

		QObject::connect(dynamic, SIGNAL(clicked(bool)), SLOT(addLhsToFd(bool)));

		lhsCheckBox.push_back(dynamic);
	}
	ui.scrollArea->setLayout(lay);

	QVBoxLayout *lay2 = new QVBoxLayout(this);
	lay2->setDirection(QBoxLayout::LeftToRight);
	for(int i=0; i < numAttributes; i++)
	{
		QCheckBox* dynamic = new QCheckBox("");
		string name = "rhsAttr";
		name += std::to_string(static_cast<long long>(i + 1));
		dynamic->setObjectName(QString(name.c_str()));
		qDebug() <<QString(name.c_str());
		dynamic->setChecked (false);
		dynamic->setText (QString(getTextOfCheckbox(i).c_str()));
		lay2->addWidget(dynamic);

		QObject::connect(dynamic, SIGNAL(clicked(bool)), SLOT(addRhsToFd(bool)));

		rhsCheckBox.push_back(dynamic);
	}
	ui.scrollArea2->setLayout(lay2);
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

