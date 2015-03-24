#ifndef CASETOOL_H
#define CASETOOL_H

#include <QtWidgets/QMainWindow>
#include <QDialog>
#include <QListWidget>
#include "ui_casetool.h"
#include "functionaldependency.h"

class CaseTool : public QMainWindow
{
	Q_OBJECT

public:
	CaseTool(QWidget *parent = 0);
	~CaseTool();

public slots:
	void addFD();
	QListWidgetItem* showFD(FunctionalDependency fd);
	void addLhsToFd(bool);
	void addRhsToFd(bool);
	void numOfAttributes();
	void reset();
	void deleteSelectedFD();
	void runLTK();
	void runNormalFormTester();
	void runAttributeClosure();
	void runBernstein();
	void renameAttr(const QString & text);
	string displayFD(FunctionalDependency fd);
	void doneEditingName();
	
private:
	Ui::CaseToolClass ui;
};

#endif // CASETOOL_H
