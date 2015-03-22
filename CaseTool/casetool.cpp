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
#include "normaltest.h"
#include "ltk.h"
#include "relation.h"

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


string getTextOfCheckbox(int i) {
	char i_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i];
	std::stringstream ss;
	ss << i_char;
	string stringToDisplay;
	ss >> stringToDisplay;
	return stringToDisplay;
}


string CaseTool::displayFD(FunctionalDependency fd) {

	string lhsStr;
	for (auto iter = lhs.begin(); iter != lhs.end(); ++iter) {
		string str = getTextOfCheckbox(static_cast<int>(*iter));
		lhsStr += str;
	}

	string rhsStr;
	for (auto iter = rhs.begin(); iter != rhs.end(); ++iter) {
		string str = getTextOfCheckbox(static_cast<int>(*iter));
		rhsStr += str;
	}

	string fullStr = lhsStr + "->" + rhsStr;

	return fullStr;
}

void CaseTool::showFD(FunctionalDependency fd) {
	string ans = displayFD(fd);
	QListWidgetItem *item = new QListWidgetItem(QString(ans.c_str()), ui.FDList);
	
	ui.FDList->setCurrentItem(item);


}

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



void CaseTool::numOfAttributes() {
	numAttributes = ui.numOfAttributes->value();

	if (ui.scrollArea->layout() != NULL) {
		clearLayout(ui.scrollArea->layout());
		lhsCheckBox.clear();
		lhs.clear();

		delete ui.scrollArea->layout();
	}

	if (ui.scrollArea2->layout() != NULL) {
		clearLayout(ui.scrollArea2->layout());
		rhsCheckBox.clear();
		rhs.clear();

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


void CaseTool::reset() {
	functionalDependecies.clear();

	ui.FDList->clear();
	ui.outputList->clear();
	ui.outputLTK->clear();
	ui.outputNF->clear();
	numOfAttributes();
}

void CaseTool::runLTK() {
	ui.outputLTK->clear();
	set<FunctionalDependency> fdSet = functionalDependecies;

	string step1Separator = "Step 1: Construct preparatory schema";
	QListWidgetItem *item = new QListWidgetItem(QString(step1Separator.c_str()), ui.outputLTK);
	item->setData(Qt::UserRole, QString(step1Separator.c_str()));
	set<Relation> preparatoryRelations = ltk::constructPreparatorySchema(fdSet, numAttributes);

	for (auto relItr = preparatoryRelations.begin(); relItr != preparatoryRelations.end(); ++relItr) {
		Relation rel = *relItr;
		string attrStr = rel.getAttributes().toString();
		item = new QListWidgetItem(QString(attrStr.c_str()), ui.outputLTK);
		item->setData(Qt::UserRole, QString(attrStr.c_str()));
		ui.outputLTK->setCurrentItem(item);
	}

	string step2Separator = "\nStep 2: Construct synthesized FDs";
	item = new QListWidgetItem(QString(step2Separator.c_str()), ui.outputLTK);
	item->setData(Qt::UserRole, QString(step2Separator.c_str()));
	unordered_map<AttributeSet, set<FunctionalDependency> > synFD = ltk::createSynthesizedFDs(preparatoryRelations);

	for (auto synFdItr = synFD.begin(); synFdItr != synFD.end(); ++ synFdItr) {
		set<FunctionalDependency> fdSet = synFdItr->second;
		for (auto fdItr = fdSet.begin(); fdItr != fdSet.end(); ++fdItr) {
			FunctionalDependency fd = *fdItr;
			string fdStr = fd.display();
			item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputLTK);
			item->setData(Qt::UserRole, QString(fdStr.c_str()));
			ui.outputLTK->setCurrentItem(item);
		}
	}

	string step3Separator = "\nStep 3: Detect superfluous attributes in each relation";
	item = new QListWidgetItem(QString(step3Separator.c_str()), ui.outputLTK);
	item->setData(Qt::UserRole, QString(step3Separator.c_str()));

	vector<Relation> prepRelations(preparatoryRelations.begin(), preparatoryRelations.end());
	for (unsigned int i = 0; i < prepRelations.size(); i++) {
		Relation rel = prepRelations[i];
		AttributeSet attributes = rel.getAttributes();
		set<int> attrSet = attributes.getAttributes();
		vector<int> attrVec(attrSet.begin(), attrSet.end());

		for (unsigned int j = 0; j < attrVec.size(); j++) {
			string step3;
			step3 += "Check if ";
			step3 += "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[attrVec[j]];
			step3 += " is superfluous in " + attributes.toString();
			item = new QListWidgetItem(QString(step3.c_str()), ui.outputLTK);
			item->setData(Qt::UserRole, QString(step3.c_str()));

			set<int> singleAttrSet;
			singleAttrSet.insert(attrVec[j]);
			AttributeSet singleAttr(singleAttrSet);
			set<AttributeSet> returnedKeys = ltk::superfluousAttributeDetection(rel, singleAttr);

			if (returnedKeys.size() == 0) {
				string notSuperfluous;
				notSuperfluous += "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[attrVec[j]];
				notSuperfluous += " is not superfluous";
				item = new QListWidgetItem(QString(notSuperfluous.c_str()), ui.outputLTK);
				item->setData(Qt::UserRole, QString(notSuperfluous.c_str()));
			} else {
				string superfluous;
				superfluous += "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[attrVec[j]];
				superfluous += " is superfluous";
				item = new QListWidgetItem(QString(superfluous.c_str()), ui.outputLTK);
				item->setData(Qt::UserRole, QString(superfluous.c_str()));

				AttributeSet finalAttributes = attributes - singleAttr;
				attributes = attributes - singleAttr;
				Relation newRelation(finalAttributes, returnedKeys);
				std::swap(newRelation, prepRelations[i]);
				//i--;  //Rerun the algorithm again
				//break;
			}
		}
	}

	string step4Separator = "\nFinal Relations:";
	item = new QListWidgetItem(QString(step4Separator.c_str()), ui.outputLTK);
	item->setData(Qt::UserRole, QString(step4Separator.c_str()));

	for (unsigned int i = 0; i < prepRelations.size(); i++) {
		Relation rel = prepRelations[i];
		AttributeSet attributes = rel.getAttributes();
		set<AttributeSet> keys = rel.getKeys();
		string attrStr = "R" + std::to_string(static_cast<long long>(i+1)) + ": " + attributes.toString();
		string keyStr;

		item = new QListWidgetItem(QString(attrStr.c_str()), ui.outputLTK);
		item->setData(Qt::UserRole, QString(attrStr.c_str()));
		keyStr = "Key(s): ";

		for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr) {
			AttributeSet key = *keyItr;
			keyStr += key.toString() + ", ";
		}
		keyStr.resize(keyStr.size() - 2);
		item = new QListWidgetItem(QString(keyStr.c_str()), ui.outputLTK);
		item->setData(Qt::UserRole, QString(keyStr.c_str()));
	}
}

void CaseTool::runNormalFormTester() {
	ui.outputNF->clear();

	set<FunctionalDependency> fdSet = functionalDependecies;
	set<int> setOfAllAttrs;
	for (int i = 0 ; i < numAttributes; i++) {
		setOfAllAttrs.insert(i);
	}

	AttributeSet attrSet (setOfAllAttrs);

	char ans = normalTest::rootProcess(fdSet, attrSet);
	string nf = "bug";
	if (ans == '1') {
		nf = "1NF";
	} else if (ans == '2') {
		nf = "2NF";
	} else if (ans == '3') {
		nf = "3NF";
	} else if (ans == 'b') {
		nf = "BCNF";
	} else if (ans == 'e'){
		nf = "EKNF";
	}

	QListWidgetItem *item = new QListWidgetItem(QString(nf.c_str()), ui.outputNF);
	item->setData(Qt::UserRole, ans);

	ui.outputNF->setCurrentItem(item);
}

AttributeSet getSmallestKey(set<AttributeSet> candidateKeys) {
	AttributeSet answer;
	bool isSet = false;

	for (auto iter = candidateKeys.begin(); iter != candidateKeys.end(); ++iter) {
		if (!isSet || iter->getAttributes().size() < answer.getAttributes().size()) {
			answer = *iter;
			isSet = true;
			qDebug() << " setting answer to size "  << answer.getAttributes().size();
		}
	}

	return answer;

}

AttributeSet fullAttributeSet() {
	set<int> fullAttr;
	for (int i = 0; i < numAttributes; i++) {
		fullAttr.insert(i);
	}
	return fullAttr;
}


void debugCandidateKeys(set<AttributeSet> candidateKeys) {
	string attrsStr;
	for (auto iter = candidateKeys.begin(); iter != candidateKeys.end(); ++iter) {
		set<int> keysAttr = iter->getAttributes();
		for (auto iter2 = keysAttr.begin(); iter2 != keysAttr.end(); ++iter2) {
			attrsStr += std::to_string(static_cast<long long>(*iter2)) ;
		}
		attrsStr += ", ";
	}
	qDebug() << "candidate keys are " << QString(attrsStr.c_str());

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
	string step2Separator = "\nStep 2:";
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
	string step3Separator = "\nStep 3:";
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
	string step4Separator = "\nStep 4:";
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
		QListWidgetItem *item = new QListWidgetItem(QString("--------"), ui.outputList);

		ui.outputList->setCurrentItem(item);
	}

	// step 5
	string step5Separator = "\nStep 5:";
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
	string step6Separator = "\nStep 6:";
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
		attrsStr += "  keys are: ";
		for (auto iter = keys.begin(); iter != keys.end(); ++iter) {
			set<int> keysAttr = iter->getAttributes();
			for (auto iter2 = keysAttr.begin(); iter2 != keysAttr.end(); ++iter2) {
				attrsStr += std::to_string(static_cast<long long>(*iter2)) ;
			}
			attrsStr += ", ";
		}

		QListWidgetItem *item = new QListWidgetItem(QString(attrsStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(attrsStr.c_str()));

		ui.outputList->setCurrentItem(item);
	}

	string step8Separator = "\nFinal set of relation, finding all candidate keys";
	item = new QListWidgetItem(QString(step8Separator.c_str()), ui.outputList);		
	ui.outputList->setCurrentItem(item);


	set<AttributeSet> keysForRelation;  // set of attributes that form key for at least one relation

	// step 8 : find all keys for every relation
	for (auto iter = finalAnswer.begin(); iter != finalAnswer.end(); ++iter) {
		// extension: find all keys of the relation
		AttributeSet attrSet = iter->first;
		set<AttributeSet> candidateKeys = bernstein::findCandidateKeys(attrSet, allFdAfterPartitioning);

		keysForRelation.insert(candidateKeys.begin(), candidateKeys.end());

		qDebug() << "-----";
		debugCandidateKeys(candidateKeys);

		set<int> attrs = iter->first.getAttributes();
		string attrsStr = "";
		for (auto iter = attrs.begin(); iter != attrs.end(); ++iter) {
			attrsStr += std::to_string(static_cast<long long>(*iter));
		}
		attrsStr += "  keys are: ";

		for (auto iter = candidateKeys.begin(); iter != candidateKeys.end(); ++iter) {
			set<int> keysAttr = iter->getAttributes();
			
			for (auto iter2 = keysAttr.begin(); iter2 != keysAttr.end(); ++iter2) {
				attrsStr += std::to_string(static_cast<long long>(*iter2)) ;
			}

			attrsStr += ", ";
		}

		item = new QListWidgetItem(QString(attrsStr.c_str()), ui.outputList);		
		ui.outputList->setCurrentItem(item);	

		
	}

	// step 7
	string step7Separator = "\nStep 7: add additional relation";
	string attrsStr;

	qDebug() << "=============================";
	set<AttributeSet> candidateKeys = bernstein::findCandidateKeys(fullAttributeSet(), allFdAfterPartitioning);


	AttributeSet smallestKey = getSmallestKey(candidateKeys);
	set<int> attrs = smallestKey.getAttributes();
	
	std::pair<AttributeSet, set<AttributeSet> > extraRelation = bernstein::constructMissingAttrRelation(finalAnswer, numAttributes, smallestKey);
		
	attrs = extraRelation.first.getAttributes();
	attrsStr = "";
	for (auto iter = attrs.begin(); iter != attrs.end(); ++iter) {
		attrsStr += std::to_string(static_cast<long long>(*iter));
	}
	// check if the extra relation is a key to some relation
	if (keysForRelation.count(extraRelation.first) == 0) {
		item = new QListWidgetItem(QString(step7Separator.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(step7Separator.c_str()));
		ui.outputList->setCurrentItem(item);

		item = new QListWidgetItem(QString(attrsStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(attrsStr.c_str()));

		ui.outputList->setCurrentItem(item);
	}

	
}

