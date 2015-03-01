/********************************************************************************
** Form generated from reading UI file 'casetool.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CASETOOL_H
#define UI_CASETOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CaseToolClass
{
public:
    QWidget *centralWidget;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QListView *listView;
    QPushButton *computeButton;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QCheckBox *checkBox_8;
    QCheckBox *checkBox_9;
    QCheckBox *checkBox_10;
    QCheckBox *checkBox_11;
    QCheckBox *checkBox_12;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CaseToolClass)
    {
        if (CaseToolClass->objectName().isEmpty())
            CaseToolClass->setObjectName(QStringLiteral("CaseToolClass"));
        CaseToolClass->resize(600, 400);
        centralWidget = new QWidget(CaseToolClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(40, 110, 70, 17));
        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(110, 110, 70, 17));
        listView = new QListView(centralWidget);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(10, 190, 511, 151));
        computeButton = new QPushButton(centralWidget);
        computeButton->setObjectName(QStringLiteral("computeButton"));
        computeButton->setGeometry(QRect(50, 150, 75, 23));
        checkBox_3 = new QCheckBox(centralWidget);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(180, 110, 70, 17));
        checkBox_4 = new QCheckBox(centralWidget);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setGeometry(QRect(250, 110, 70, 17));
        checkBox_5 = new QCheckBox(centralWidget);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        checkBox_5->setGeometry(QRect(320, 110, 70, 17));
        checkBox_6 = new QCheckBox(centralWidget);
        checkBox_6->setObjectName(QStringLiteral("checkBox_6"));
        checkBox_6->setGeometry(QRect(390, 110, 70, 17));
        checkBox_7 = new QCheckBox(centralWidget);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));
        checkBox_7->setGeometry(QRect(250, 60, 70, 17));
        checkBox_8 = new QCheckBox(centralWidget);
        checkBox_8->setObjectName(QStringLiteral("checkBox_8"));
        checkBox_8->setGeometry(QRect(40, 60, 70, 17));
        checkBox_9 = new QCheckBox(centralWidget);
        checkBox_9->setObjectName(QStringLiteral("checkBox_9"));
        checkBox_9->setGeometry(QRect(390, 60, 70, 17));
        checkBox_10 = new QCheckBox(centralWidget);
        checkBox_10->setObjectName(QStringLiteral("checkBox_10"));
        checkBox_10->setGeometry(QRect(110, 60, 70, 17));
        checkBox_11 = new QCheckBox(centralWidget);
        checkBox_11->setObjectName(QStringLiteral("checkBox_11"));
        checkBox_11->setGeometry(QRect(180, 60, 70, 17));
        checkBox_12 = new QCheckBox(centralWidget);
        checkBox_12->setObjectName(QStringLiteral("checkBox_12"));
        checkBox_12->setGeometry(QRect(320, 60, 70, 17));
        CaseToolClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CaseToolClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        CaseToolClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CaseToolClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CaseToolClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CaseToolClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CaseToolClass->setStatusBar(statusBar);

        retranslateUi(CaseToolClass);

        QMetaObject::connectSlotsByName(CaseToolClass);
    } // setupUi

    void retranslateUi(QMainWindow *CaseToolClass)
    {
        CaseToolClass->setWindowTitle(QApplication::translate("CaseToolClass", "CaseTool", 0));
        checkBox->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_2->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        computeButton->setText(QApplication::translate("CaseToolClass", "Compute", 0));
        checkBox_3->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_4->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_5->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_6->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_7->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_8->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_9->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_10->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_11->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
        checkBox_12->setText(QApplication::translate("CaseToolClass", "CheckBox", 0));
    } // retranslateUi

};

namespace Ui {
    class CaseToolClass: public Ui_CaseToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASETOOL_H
