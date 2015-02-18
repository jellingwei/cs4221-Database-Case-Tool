#include "casetool.h"
#include <QtWidgets/QApplication>
#include <QDebug> 

#include <iostream>
#include "attributeset.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CaseTool w;
	w.show();

	return a.exec();
}
