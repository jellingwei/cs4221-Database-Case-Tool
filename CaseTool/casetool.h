#ifndef CASETOOL_H
#define CASETOOL_H

#include <QtWidgets/QMainWindow>
#include <QDialog>
#include <QListWidget>
#include "ui_casetool.h"

class CaseTool : public QMainWindow
{
	Q_OBJECT

public:
	CaseTool(QWidget *parent = 0);
	~CaseTool();

public slots:
	void addFD();
	void addLhsToFd(bool);
	void addRhsToFd(bool);
	void numOfAttributes(int num);
	void runBernstein();
	
private:
	Ui::CaseToolClass ui;
};

#endif // CASETOOL_H
