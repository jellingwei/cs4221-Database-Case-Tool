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
	void showFD(FunctionalDependency fd);
	void addLhsToFd(bool);
	void addRhsToFd(bool);
	void numOfAttributes();
	void reset();
	void runLTK();
	void runNormalFormTester();
	void runBernstein();
	string displayFD(FunctionalDependency fd);
	
private:
	Ui::CaseToolClass ui;
};

#endif // CASETOOL_H
