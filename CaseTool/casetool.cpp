#include <set>
#include <unordered_map>
#include <map>
#include <sstream>

#include <QVBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
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
using std::map;

CaseTool::CaseTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.numOfAttributes->setValue(5);

}

CaseTool::~CaseTool()
{

}


set<int> lhs;
set<int> rhs;
int numAttributes;

vector<QCheckBox*> lhsCheckBox;
vector<QCheckBox*> rhsCheckBox;
vector<QLineEdit*> attrLineEdit;

set<FunctionalDependency> functionalDependecies;
unordered_map<string, FunctionalDependency> fdStrToFd;
map<FunctionalDependency, QListWidgetItem*> fdToDisplayedItem;
vector<string> attrNames;

string attrLineEditObjectName = "attrName";

string getTextOfCheckbox(int i) {

	if (attrNames.size() > i && attrNames[i] != "") {
		return attrNames[i];
	} else {
		char i_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i];
		std::stringstream ss;
		ss << i_char;
		string stringToDisplay;
		ss >> stringToDisplay;
		return stringToDisplay;
	}
}


string CaseTool::displayFD(FunctionalDependency fd) {

	string lhsStr ="";
	set<int> lhs = fd.getLhs();
	for (auto iter = lhs.begin(); iter != lhs.end(); ++iter) {
		if (lhsStr != "") {
			lhsStr += ",";
		}
		string str = getTextOfCheckbox(static_cast<long long>(*iter));
		lhsStr += str;
	}

	string rhsStr ="";
	set<int> rhs = fd.getRhs();
	for (auto iter = rhs.begin(); iter != rhs.end(); ++iter) {
		if (rhsStr != "") {
			rhsStr += ",";
		}
		string str = getTextOfCheckbox(static_cast<long long>(*iter));
		rhsStr += str;
	}

	string fullStr = lhsStr + "->" + rhsStr;

	return fullStr;
}

string displayAttributeSet(AttributeSet attrSet) {
	set<int> attr = attrSet.getAttributes();
	string finalStr = "";
	for (auto iter = attr.begin(); iter != attr.end(); ++iter) {
		if (finalStr != "") {
			finalStr += ",";
		}
		string str = getTextOfCheckbox(static_cast<long long>(*iter));
		finalStr += str;
	}

	return "(" + finalStr + ")";
}

QListWidgetItem* CaseTool::showFD(FunctionalDependency fd) {
	string ans = displayFD(fd);
	QListWidgetItem *item = new QListWidgetItem(QString(ans.c_str()), ui.FDList);

	fdStrToFd[ans] = fd;

	ui.FDList->setCurrentItem(item);
	return item;
}

void CaseTool::addFD() {
	FunctionalDependency fd(lhs, rhs);
	functionalDependecies.insert(fd);


	if(!lhs.empty() && !rhs.empty()) {
		fdToDisplayedItem[fd] = showFD(fd);
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
		} else if (!isChecked && name==qstr) {
			lhs.erase(i);
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
		} else if (!isChecked && name==qstr) {
			rhs.erase(i);
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

void CaseTool::renameAttr(const QString & text) {
	QObject* obj = sender();
	QLineEdit* emittingLineEdit = dynamic_cast<QLineEdit*>(obj);

	QString name = emittingLineEdit->objectName();	

	for (int i = 0; i < numAttributes; i++) {
		string attrName = attrLineEditObjectName;
		attrName += std::to_string(static_cast<long long>(i + 1));

		QCheckBox* leftCheckBox = lhsCheckBox[i];
		QCheckBox* rightCheckBox = rhsCheckBox[i];

		if (name == QString::fromStdString(attrName)) {
			leftCheckBox->setText(text);
			rightCheckBox->setText(text);

			string oldAttrName = attrNames[i];
			attrNames[i] = text.toStdString();

			for (auto iter = functionalDependecies.begin(); iter != functionalDependecies.end(); ++iter) {
				FunctionalDependency fd = *iter;

				set<int> setWithUpdatedName;  setWithUpdatedName.insert(i);
				if (fd.getLhsAttrSet().containsAttributes(setWithUpdatedName) || 
					fd.getRhsAttrSet().containsAttributes(setWithUpdatedName)) {

						// change display of FD
						QListWidgetItem* item = fdToDisplayedItem[fd];

						item->setText( QString::fromStdString(displayFD(fd) ) );
						fdStrToFd.erase(oldAttrName);
						fdStrToFd[displayFD(fd)] = fd;
				}

			}
			break;
		}
	}

}

void CaseTool::doneEditingName() {
	QObject* obj = sender();
	QLineEdit* emittingLineEdit = dynamic_cast<QLineEdit*>(obj);

	QString name = emittingLineEdit->objectName();	
	QString attributeName = emittingLineEdit->text();

	bool isRepeated= false;
	int editedIndex;

	for (int i = 0; i < numAttributes; i++) {
		// if the name is repeated from the current names
		QString attributeObjectName = QString::fromStdString(attrLineEditObjectName + std::to_string(static_cast<long long>(i + 1)));
		if (getTextOfCheckbox(i) == attributeName.toStdString() && name != attributeObjectName) {
			isRepeated = true;
		}

		if (name == attributeObjectName) {
			editedIndex = i;
		}

	}

	if (isRepeated) {
		attrNames[editedIndex] = ""; // reset
		emittingLineEdit->setText(QString::fromStdString(getTextOfCheckbox(editedIndex) ));
	}

}

void CaseTool::numOfAttributes() {
	numAttributes = ui.numOfAttributes->value();


	if (ui.attrNameScrollArea->layout() != NULL) {
		clearLayout(ui.attrNameScrollArea->layout());
		attrLineEdit.clear();

		delete ui.attrNameScrollArea->layout();
	}

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


	QVBoxLayout *attrNamelay = new QVBoxLayout(this);
	attrNamelay->setDirection(QBoxLayout::LeftToRight);
	vector<string> tempAttrNames;
	for(int i=0; i < numAttributes; i++)
	{
		QLineEdit* dynamic = new QLineEdit("");
		string name = attrLineEditObjectName;
		name += std::to_string(static_cast<long long>(i + 1));

		dynamic->setObjectName(QString(name.c_str()));
		qDebug() <<QString(name.c_str());

		dynamic->setText (QString(getTextOfCheckbox(i).c_str()));
		attrNamelay->addWidget(dynamic);

		QObject::connect(dynamic, SIGNAL(textChanged(const QString &)), SLOT(renameAttr(const QString &)));

		QObject::connect(dynamic, SIGNAL(editingFinished()), SLOT(doneEditingName()));


		attrLineEdit.push_back(dynamic);
		if (attrNames.size() > i && attrNames[i] != "") {
			tempAttrNames.push_back(attrNames[i]);
		} else {
			tempAttrNames.push_back("");
		}
	}

	attrNames = tempAttrNames;

	ui.attrNameScrollArea->setLayout(attrNamelay);

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

void CaseTool::deleteSelectedFD() {
	QList<QListWidgetItem*> list =  ui.FDList->selectedItems();
	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		QListWidgetItem* item = *iter;
		QString fdStr = item->text();		

		FunctionalDependency fd = fdStrToFd[fdStr.toStdString()];
		functionalDependecies.erase(fd);
	}

	qDeleteAll(ui.FDList->selectedItems());

}

void CaseTool::runLTK() {
	ui.outputLTK->clear();
	set<FunctionalDependency> fdSet = functionalDependecies;

	string step1Separator = "Step 1: Construct preparatory schema";
	QListWidgetItem *item = new QListWidgetItem(QString(step1Separator.c_str()), ui.outputLTK);
	item->setData(Qt::UserRole, QString(step1Separator.c_str()));
	set<Relation> preparatoryRelations = ltk::constructPreparatorySchema(fdSet, numAttributes);

	int counter = 1;
	for (auto relItr = preparatoryRelations.begin(); relItr != preparatoryRelations.end(); ++relItr) {
		Relation rel = *relItr;
		AttributeSet attributes = rel.getAttributes();
		set<AttributeSet> keys = rel.getKeys();
		string attrStr = "R" + std::to_string(static_cast<long long>(counter++)) + ": " + displayAttributeSet(attributes);
		item = new QListWidgetItem(QString(attrStr.c_str()), ui.outputLTK);
		item->setData(Qt::UserRole, QString(attrStr.c_str()));
		ui.outputLTK->setCurrentItem(item);
		
		string keyStr = "Key(s): ";
		for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr) {
			AttributeSet key = *keyItr;
			keyStr += displayAttributeSet(key) + ", ";
		}
		keyStr.resize(keyStr.size() - 2);
		item = new QListWidgetItem(QString(keyStr.c_str()), ui.outputLTK);
		item->setData(Qt::UserRole, QString(keyStr.c_str()));
	}

	string step2Separator = "\nStep 2: Construct synthesized FDs";
	item = new QListWidgetItem(QString(step2Separator.c_str()), ui.outputLTK);
	item->setData(Qt::UserRole, QString(step2Separator.c_str()));
	unordered_map<AttributeSet, set<FunctionalDependency> > synFD = ltk::createSynthesizedFDs(preparatoryRelations);

	for (auto synFdItr = synFD.begin(); synFdItr != synFD.end(); ++ synFdItr) {
		set<FunctionalDependency> fdSet = synFdItr->second;
		for (auto fdItr = fdSet.begin(); fdItr != fdSet.end(); ++fdItr) {
			FunctionalDependency fd = *fdItr;
			string fdStr = displayFD(fd);
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

		string output = "\nChecking " + displayAttributeSet(attributes) + " for superfluous attributes";
		item = new QListWidgetItem(QString(output.c_str()), ui.outputLTK);
		item->setData(Qt::UserRole, QString(output.c_str()));

		for (unsigned int j = 0; j < attrVec.size(); j++) {
			rel = prepRelations[i];
			attributes = rel.getAttributes();
			set<int> singleAttrSet;
			singleAttrSet.insert(attrVec[j]);
			AttributeSet singleAttr(singleAttrSet);

			string step3;
			step3 += "Check if " + displayAttributeSet(singleAttrSet) + 
				" is superfluous in " + displayAttributeSet(attributes);
			item = new QListWidgetItem(QString(step3.c_str()), ui.outputLTK);
			item->setData(Qt::UserRole, QString(step3.c_str()));

			set<AttributeSet> returnedKeys = ltk::superfluousAttributeDetection(rel, singleAttr);

			if (returnedKeys.size() == 0) {
				string notSuperfluous;
				notSuperfluous += displayAttributeSet(singleAttrSet) + " is not superfluous";
				item = new QListWidgetItem(QString(notSuperfluous.c_str()), ui.outputLTK);
				item->setData(Qt::UserRole, QString(notSuperfluous.c_str()));
			} else {
				/*set<FunctionalDependency> synthesizedPrimeFDs = ltk::getSynthesizedPrimeFDs();
				step3 = "Construct G'(" + displayAttributeSet(singleAttr) + ")";
				item = new QListWidgetItem(QString(step3.c_str()), ui.outputLTK);
				item->setData(Qt::UserRole, QString(step3.c_str()));

				for (auto fdItr = synthesizedPrimeFDs.begin(); fdItr != synthesizedPrimeFDs.end(); ++fdItr) {
					step3 = displayFD(*fdItr);
					item = new QListWidgetItem(QString(step3.c_str()), ui.outputLTK);
					item->setData(Qt::UserRole, QString(step3.c_str()));
				}*/

				string superfluous;
				superfluous += displayAttributeSet(singleAttrSet) + " is superfluous";
				item = new QListWidgetItem(QString(superfluous.c_str()), ui.outputLTK);
				item->setData(Qt::UserRole, QString(superfluous.c_str()));

				//Since single attribute is superfluous, remove it
				AttributeSet finalAttributes = attributes - singleAttr;
				Relation newRelation(finalAttributes, returnedKeys);
				preparatoryRelations.erase(rel);  //Erase the old relation from the set
				preparatoryRelations.insert(newRelation);  //Insert the reconstructed relation
				std::swap(newRelation, prepRelations[i]);  //Swap the relations in the vector

				//Print message to state which attribute is removed
				string remove = "Remove " + displayAttributeSet(singleAttrSet) + " to get " + displayAttributeSet(finalAttributes);
				item = new QListWidgetItem(QString(remove.c_str()), ui.outputLTK);
				item->setData(Qt::UserRole, QString(remove.c_str()));

				//Since the relation is reconstructed, construct another set of synthesized FDs
				unordered_map<AttributeSet, set<FunctionalDependency> > synFD = ltk::createSynthesizedFDs(preparatoryRelations);
				string message = "Create new set of synthesized FDs";
				item = new QListWidgetItem(QString(message.c_str()), ui.outputLTK);
				item->setData(Qt::UserRole, QString(message.c_str()));
				ui.outputLTK->setCurrentItem(item);

				for (auto synFdItr = synFD.begin(); synFdItr != synFD.end(); ++ synFdItr) {
					set<FunctionalDependency> fdSet = synFdItr->second;
					for (auto fdItr = fdSet.begin(); fdItr != fdSet.end(); ++fdItr) {
						FunctionalDependency fd = *fdItr;
						string fdStr = displayFD(fd);
						item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputLTK);
						item->setData(Qt::UserRole, QString(fdStr.c_str()));
						ui.outputLTK->setCurrentItem(item);
					}
				}
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
		string attrStr = "R" + std::to_string(static_cast<long long>(i+1)) + ": " + displayAttributeSet(attributes);
		item = new QListWidgetItem(QString(attrStr.c_str()), ui.outputLTK);
		item->setData(Qt::UserRole, QString(attrStr.c_str()));

		string keyStr = "Key(s): ";

		/*Extension: Find all keys of the relation
		unordered_map<AttributeSet, set<FunctionalDependency> > synFD = ltk::createSynthesizedFDs(preparatoryRelations);
		set<FunctionalDependency> allSynFD;
		for (auto itr = synFD.begin(); itr != synFD.end(); ++itr) {
			set<FunctionalDependency> fdSet = itr->second;
			for (auto fdItr = fdSet.begin(); fdItr != fdSet.end(); ++fdItr) {
				allSynFD.insert(*fdItr);
			}
		}
		set<AttributeSet> candidateKeys = bernstein::findCandidateKeys(attributes, allSynFD);

		item = new QListWidgetItem(QString(attrStr.c_str()), ui.outputLTK);
		item->setData(Qt::UserRole, QString(attrStr.c_str()));
		keyStr = "Key(s): ";*/

		/*
		for (auto keyItr = candidateKeys.begin(); keyItr != candidateKeys.end(); ++keyItr) {
			AttributeSet key = *keyItr;
			keyStr += displayAttributeSet(key) + ", ";
		}*/
		for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr) {
			AttributeSet key = *keyItr;
			keyStr += displayAttributeSet(key) + ", ";
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
	normalTest::resetState();
	char ans = normalTest::normalTestRootProcess(fdSet, attrSet);
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
	} else if (ans == 'f'){
		nf = "BCNF (No FDs)";
	}

	QListWidgetItem *item = new QListWidgetItem(QString(nf.c_str()), ui.outputNF);
	item->setData(Qt::UserRole, ans);

	ui.outputNF->setCurrentItem(item);
}

void CaseTool::runAttributeClosure() {
	AttributeSet startingSet = AttributeSet(lhs);
	ui.outputAttrClosure->clear();

	if (startingSet.size() == 0) {
		QListWidgetItem *item = new QListWidgetItem("Enter the attributes to find a closure for under LHS", ui.outputAttrClosure);
		return;
	}

	AttributeSet closure = startingSet.getAttributeClosure(functionalDependecies);

	QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(displayAttributeSet(closure)), ui.outputAttrClosure);

	ui.outputAttrClosure->setCurrentItem(item);
}

AttributeSet getSmallestKey(set<AttributeSet> candidateKeys) {
	AttributeSet answer;
	bool isSet = false;

	for (auto iter = candidateKeys.begin(); iter != candidateKeys.end(); ++iter) {
		if (!isSet || iter->getAttributes().size() < answer.getAttributes().size()) {
			answer = *iter;
			isSet = true;
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
	string step1Separator = "Step 1: Remove redundant attributes";
	QListWidgetItem *item = new QListWidgetItem(QString(step1Separator.c_str()), ui.outputList);
	item->setData(Qt::UserRole, QString(step1Separator.c_str()));

	ui.outputList->setCurrentItem(item);
	set<FunctionalDependency> newFdSet = bernstein::removeRedundantAttributes(fdSet);

	vector<std::pair<int, FunctionalDependency> > messageForStep1 = bernstein::getMessageForStep1();
	for (auto iter = messageForStep1.begin(); iter != messageForStep1.end(); ++iter) {
		int redundantAttribute = iter->first;
		FunctionalDependency fd = iter->second;

		string step1Explanation = "The attribute " + getTextOfCheckbox(redundantAttribute) + " is redundant in the FD " + displayFD(fd);
		item = new QListWidgetItem(QString(step1Explanation.c_str()), ui.outputList);
	}

	for (auto iter = newFdSet.begin(); iter != newFdSet.end(); ++iter) {
		FunctionalDependency fd = *iter;

		string fdStr = displayFD(fd);

		QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(fdStr.c_str()));

		ui.outputList->setCurrentItem(item);
	}

	// step 2...
	string step2Separator = "\nStep 2: Find covering";
	QListWidgetItem *stepWidget2 = new QListWidgetItem(QString(step2Separator.c_str()), ui.outputList);
	//item->setData(Qt::UserRole, QString(step2Separator.c_str()));

	ui.outputList->setCurrentItem(stepWidget2);
	set<FunctionalDependency> minimalCover = bernstein::obtainMinimalCover(fdSet);

	vector<FunctionalDependency> messageForStep2 = bernstein::getMessageForRemovingTransitiveDependencies();
	for (auto iter = messageForStep2.begin(); iter != messageForStep2.end(); ++iter) {
		string step2Explanation = "The FD " + displayFD(*iter) + " is redundant.";
		item = new QListWidgetItem(QString(step2Explanation.c_str()), ui.outputList);
	}

	for (auto iter = minimalCover.begin(); iter != minimalCover.end(); ++iter) {
		FunctionalDependency fd = *iter;
		string fdStr = displayFD(fd);
		QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(fdStr.c_str()));

		ui.outputList->setCurrentItem(item);
	}

	// step 3
	string step3Separator = "\nStep 3: Partition";
	QListWidgetItem *stepWidget3 = new QListWidgetItem(QString(step3Separator.c_str()), ui.outputList);
	ui.outputList->setCurrentItem(stepWidget3);

	item = new QListWidgetItem(QString("--------"), ui.outputList);
	unordered_map<AttributeSet, set<FunctionalDependency> > partitions = bernstein::partitionFd(minimalCover);
	for (auto iter = partitions.begin(); iter != partitions.end(); ++iter) {
		set<FunctionalDependency> fdSet = iter->second;

		item = new QListWidgetItem(QString(("Partition for " + displayAttributeSet(iter->first) + ":").c_str()), ui.outputList);

		for (auto fdIter = fdSet.begin(); fdIter != fdSet.end(); ++fdIter) {
			FunctionalDependency fd = *fdIter;
			string fdStr = displayFD(fd);
			QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
			item->setData(Qt::UserRole, QString(fdStr.c_str()));

			ui.outputList->setCurrentItem(item);
		}
		QListWidgetItem *item = new QListWidgetItem(QString("--------"), ui.outputList);
	}

	// step 4
	string step4Separator = "\nStep 4: Merge groups";
	QListWidgetItem *stepWidget4 = new QListWidgetItem(QString(step4Separator.c_str()), ui.outputList);
	ui.outputList->setCurrentItem(stepWidget4);

	partitions = bernstein::mergeEquivalentKeys(partitions, minimalCover);
	item = new QListWidgetItem(QString("--------"), ui.outputList);

	int partitionNum = 0;
	for (auto iter = partitions.begin(); iter != partitions.end(); ++iter) {
		set<FunctionalDependency> fdSet = iter->second;
		if (fdSet.size() == 0) {
			continue;
		}

		AttributeSet lhs = iter->first;
		if (lhs.size() != 0 ) {
			string partitionSeperator = string("Partition ") + std::to_string(static_cast<long long>(++partitionNum)) + string(":");
			item = new QListWidgetItem(partitionSeperator.c_str(), ui.outputList);
		} else {
			string partitionSeperator = string("Partition J:");
			item = new QListWidgetItem(partitionSeperator.c_str(), ui.outputList);
		}

		for (auto fdIter = fdSet.begin(); fdIter != fdSet.end(); ++fdIter) {
			FunctionalDependency fd = *fdIter;
			string fdStr = displayFD(fd);
			QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
			item->setData(Qt::UserRole, QString(fdStr.c_str()));

			ui.outputList->setCurrentItem(item);
		}
		QListWidgetItem *item = new QListWidgetItem(QString("--------"), ui.outputList);

		ui.outputList->setCurrentItem(item);
	}

	// step 5
	string step5Separator = "\nStep 5: Remove transitive dependencies";
	QListWidgetItem *stepWidget5 = new QListWidgetItem(QString(step5Separator.c_str()), ui.outputList);
	ui.outputList->setCurrentItem(stepWidget5);

	set<int> emptySet;

	set<FunctionalDependency> allFdAfterPartitioning = bernstein::createSetOfFDFromPartitions(partitions);
	partitions = bernstein::eliminateTransitiveDependenciesForPartition(partitions, allFdAfterPartitioning);

	vector<FunctionalDependency> messageForStep5 = bernstein::getMessageForRemovingTransitiveDependencies();
	for (auto iter = messageForStep5.begin(); iter != messageForStep5.end(); ++iter) {
		string step5Explanation = "The FD " + displayFD(*iter) + " is redundant.";
		item = new QListWidgetItem(QString(step5Explanation.c_str()), ui.outputList);
	}

	partitions = bernstein::addFdInJBackToCorrespondingGroup(partitions);

	item = new QListWidgetItem(QString("--------"), ui.outputList);
	for (auto iter = partitions.begin(); iter != partitions.end(); ++iter) {
		set<FunctionalDependency> fdSet = iter->second;

		if (fdSet.size() == 0) {
			continue;
		}

		for (auto fdIter = fdSet.begin(); fdIter != fdSet.end(); ++fdIter) {
			FunctionalDependency fd = *fdIter;
			string fdStr = displayFD(fd);
			QListWidgetItem *item = new QListWidgetItem(QString(fdStr.c_str()), ui.outputList);
			item->setData(Qt::UserRole, QString(fdStr.c_str()));

			ui.outputList->setCurrentItem(item);
		}
		QListWidgetItem *item = new QListWidgetItem(QString("--------"), ui.outputList);
	}

	// step 6
	string step6Separator = "\nStep 6: Construct Relations";
	item = new QListWidgetItem(QString(step6Separator.c_str()), ui.outputList);
	item->setData(Qt::UserRole, QString(step6Separator.c_str()));

	ui.outputList->setCurrentItem(item);
	set<std::pair<AttributeSet, set<AttributeSet> > > finalAnswer = bernstein::constructRelations(partitions);

	for (auto iter = finalAnswer.begin(); iter != finalAnswer.end(); ++iter) {
		AttributeSet attrsInRelation = iter->first;
		set<AttributeSet> keys = iter->second;


		string attrsStr;
		attrsStr = displayAttributeSet(attrsInRelation) + '\n';
		attrsStr += "Key(s) : ";
		for (auto iter = keys.begin(); iter != keys.end(); ++iter) {
			attrsStr += displayAttributeSet(*iter) + ", ";
		}
		attrsStr.resize(attrsStr.size() - 2);

		QListWidgetItem *item = new QListWidgetItem(QString(attrsStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(attrsStr.c_str()));

		ui.outputList->setCurrentItem(item);
	}

	string step8Separator = "\nFinal relations";
	item = new QListWidgetItem(QString(step8Separator.c_str()), ui.outputList);		
	ui.outputList->setCurrentItem(item);


	set<AttributeSet> keysForRelation;  // set of attributes that form key for at least one relation

	// step 8 : find all keys for every relation
	int relNum = 0;
	for (auto iter = finalAnswer.begin(); iter != finalAnswer.end(); ++iter) {
		// extension: find all keys of the relation
		AttributeSet attrSet = iter->first;
		set<AttributeSet> candidateKeys = bernstein::findCandidateKeys(attrSet, allFdAfterPartitioning);

		keysForRelation.insert(candidateKeys.begin(), candidateKeys.end());

		debugCandidateKeys(candidateKeys);


		string attrStr = "R" + std::to_string(static_cast<long long>(++relNum)) + ": " + displayAttributeSet(attrSet);
		string keyStr;

		item = new QListWidgetItem(QString(attrStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(attrStr.c_str()));
		keyStr = "Key(s): ";

		for (auto keyItr = candidateKeys.begin(); keyItr != candidateKeys.end(); ++keyItr) {
			AttributeSet key = *keyItr;
			keyStr += displayAttributeSet(key) + ", ";
		}
		keyStr.resize(keyStr.size() - 2);
		item = new QListWidgetItem(QString(keyStr.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(keyStr.c_str()));
	}

	// step 7
	string step7Separator = "\nAdd additional relation for reconstructability";
	string attrsStr;

	set<AttributeSet> candidateKeys = bernstein::findCandidateKeys(fullAttributeSet(), allFdAfterPartitioning);


	AttributeSet smallestKey = getSmallestKey(candidateKeys);
	set<int> attrs = smallestKey.getAttributes();

	std::pair<AttributeSet, set<AttributeSet> > extraRelation = bernstein::constructMissingAttrRelation(finalAnswer, numAttributes, smallestKey);

	attrs = extraRelation.first.getAttributes();
	attrsStr = displayAttributeSet(attrs);

	// check if the extra relation is a key to some relation
	if (keysForRelation.count(extraRelation.first) == 0) {
		item = new QListWidgetItem(QString(step7Separator.c_str()), ui.outputList);
		item->setData(Qt::UserRole, QString(step7Separator.c_str()));

		string attrStr = "R" + std::to_string(static_cast<long long>(++relNum)) + ": " + attrsStr;

		item = new QListWidgetItem(QString(attrStr.c_str()), ui.outputList);

		string keyStr = "Key(s): " + attrsStr;
		item = new QListWidgetItem(QString(keyStr.c_str()), ui.outputList);

		ui.outputList->setCurrentItem(item);
	}


}

