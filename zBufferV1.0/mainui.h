#ifndef MAINUI_H
#define MAINUI_H

#include <QtGui/QMainWindow>
#include <QApplication>  
#include <QAction>  
#include <QMenu>  
#include <QFileDialog>  
#include <QFile>  
#include <QTextStream>  
#include <QToolBar>  
#include <QTextEdit>  
#include <QMainWindow>  
#include <QString>  
#include <QMenuBar>
#include "glwidget.h"

#include "ui_mainui.h"


class MainUI : public QMainWindow
{
	Q_OBJECT

public:
	MainUI(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainUI();

	void createMenus();  
    void createActions();  
    void createToolBars();  
    void loadFile(QString fileName); 
signals:
	void signal_loadObj(QString filename);

public slots:  
    void slotNewFile();  
    void slotOpenFile();  
    void slotSaveFile();  

private:
	Ui::MainUIClass ui;
	GLWidget *glWidget;
	QMenu *fileMenu;  
    QMenu *editMenu;  
    QMenu *aboutMenu;  
    QString fileName;  
  
    QToolBar *fileTool;  
    QToolBar *editTool;  
    QToolBar *directionTool;
	QToolBar *rotateTool;
	QToolBar *zoomTool;

    QAction *fileOpenAction;  
    QAction *fileNewAction;  
    QAction *fileSaveAction;  
    QAction *exitAction;  
    QAction *copyAction;  
    QAction *cutAction;  
    QAction *pasteAction;  
    QAction *aboutAction;  
    QAction *leftAction;
	QAction *rightAction;
	QAction *upAction;
    QAction *downAction;
	QAction *xPAction;
	QAction *xNAction;
	QAction *yPAction;
	QAction *yNAction;
	QAction *zPAction;
	QAction *zNAction;
	QAction *zoomInAction;
	QAction *zoomOutAction;

    QTextEdit *text;  


};

#endif // MAINUI_H
