/********************************************************************************
** Form generated from reading UI file 'mainui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUI_H
#define UI_MAINUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainUIClass)
    {
        if (MainUIClass->objectName().isEmpty())
            MainUIClass->setObjectName(QString::fromUtf8("MainUIClass"));
        MainUIClass->resize(600, 400);
        menuBar = new QMenuBar(MainUIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        MainUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainUIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MainUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainUIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainUIClass->setStatusBar(statusBar);

        retranslateUi(MainUIClass);

        QMetaObject::connectSlotsByName(MainUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainUIClass)
    {
        MainUIClass->setWindowTitle(QApplication::translate("MainUIClass", "MainUI", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainUIClass: public Ui_MainUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
