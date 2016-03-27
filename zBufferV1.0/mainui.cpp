#include "mainui.h"

MainUI::MainUI(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//ui.setupUi(this);
	this->resize( QSize( 1000, 700 ));
	setWindowTitle(tr("Zbuffer"));  


	glWidget =new GLWidget();
    setCentralWidget(glWidget);  

    createActions();  
    createMenus();  
    createToolBars();  

	connect(this,SIGNAL(signal_loadObj(QString)),glWidget,SLOT(slot_receiveObj(QString)));
}

MainUI::~MainUI()
{

}

void MainUI::createActions()  
{  
    //通过图片路径来实现添加Qt资源文件目的  
    fileOpenAction = new QAction(QIcon(":/images/open.png"),tr("Open"),this);  
    fileOpenAction->setShortcut(tr("Ctrl+O"));  
    fileOpenAction->setStatusTip(tr("Open a file"));  
    connect(fileOpenAction,SIGNAL(triggered()),this,SLOT(slotOpenFile()));  
  
    fileNewAction = new QAction(QIcon(":/images/new.png"),tr("New"),this);  
    fileNewAction->setShortcut(tr("Ctrl+N"));  
    fileNewAction->setStatusTip(tr("new file"));  
    connect(fileNewAction,SIGNAL(triggered()),this,SLOT(slotNewFile()));  
  
    fileSaveAction = new QAction(QIcon(":/images/save.png"),tr("Save"),this);  
    fileSaveAction->setShortcut(tr("Ctrl+S"));  
    fileNewAction->setStatusTip(tr("save file"));  
    connect(fileNewAction,SIGNAL(triggered()),this,SLOT(slotSaveFile()));  
  
    exitAction = new QAction(tr("Exit"),this);  
    exitAction->setShortcut(tr("Ctrl+Q"));  
    exitAction->setStatusTip(tr("exit"));  
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));  
  
    cutAction = new QAction(QIcon(":/images/cut.png"),tr("Cut"),this);  
    cutAction->setShortcut(tr("Ctrl+X"));  
    cutAction->setStatusTip(tr("cut to clipboard"));  
   // connect(cutAction,SIGNAL(triggered()),text,SLOT(cut()));  
  
    copyAction = new QAction(QIcon(":/images/copy.png"),tr("Copy"),this);  
    copyAction->setShortcut(tr("Ctrl+C"));  
    copyAction->setStatusTip(tr("copy to clipboard"));  
   // connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));  
  
    pasteAction = new QAction(QIcon(":/images/paste.png"),tr("paste"),this);  
    pasteAction->setShortcut(tr("Ctrl+V"));  
    pasteAction->setStatusTip(tr("paste clipboard to selection"));  
   // connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));  
  
    aboutAction = new QAction(tr("About"),this);  
  //  connect(aboutAction,SIGNAL(triggered()),this,SLOT(slotAbout()));  
    
	leftAction=new QAction(QIcon(":/images/left.png"),tr("left"),this);
	pasteAction->setStatusTip(tr("向左移动")); 
	connect(leftAction,SIGNAL(triggered()),glWidget,SLOT(slotLeft()));  

	rightAction=new QAction(QIcon(":/images/right.png"),tr("right"),this);
	rightAction->setStatusTip(tr("向右移动")); 
	connect(rightAction,SIGNAL(triggered()),glWidget,SLOT(slotRight()));  

	upAction=new QAction(QIcon(":/images/up.png"),tr("up"),this);
	upAction->setStatusTip(tr("向上移动")); 
	connect(upAction,SIGNAL(triggered()),glWidget,SLOT(slotUp()));  

	downAction=new QAction(QIcon(":/images/down.png"),tr("down"),this);
	downAction->setStatusTip(tr("向下移动")); 
	connect(downAction,SIGNAL(triggered()),glWidget,SLOT(slotDown()));  

	xPAction=new QAction(QIcon(":/images/xp.png"),tr("Rotate around X Positive"),this);
	xPAction->setStatusTip(tr("x轴正向旋转"));
	connect(xPAction,SIGNAL(triggered()),glWidget,SLOT(slotRotateXP())); 

	xNAction=new QAction(QIcon(":/images/xn.png"),tr("Rotate around X Negative"),this);
	xNAction->setStatusTip(tr("x轴逆向旋转"));
	connect(xNAction,SIGNAL(triggered()),glWidget,SLOT(slotRotateXN())); 

	yPAction=new QAction(QIcon(":/images/yp.png"),tr("Rotate around Y Positive"),this);
	yPAction->setStatusTip(tr("y轴正向旋转"));
	connect(yPAction,SIGNAL(triggered()),glWidget,SLOT(slotRotateYP())); 

	yNAction=new QAction(QIcon(":/images/yn.png"),tr("Rotate around X Negative"),this);
	yNAction->setStatusTip(tr("y轴逆向旋转"));
	connect(yNAction,SIGNAL(triggered()),glWidget,SLOT(slotRotateYN())); 

	zPAction=new QAction(QIcon(":/images/zp.png"),tr("Rotate around Z Positive"),this);
	zPAction->setStatusTip(tr("z轴正向旋转"));
	connect(zPAction,SIGNAL(triggered()),glWidget,SLOT(slotRotateZP())); 

	zNAction=new QAction(QIcon(":/images/zn.png"),tr("Rotate around Z Negative"),this);
	zNAction->setStatusTip(tr("z轴逆向旋转"));
	connect(zNAction,SIGNAL(triggered()),glWidget,SLOT(slotRotateZN())); 

	zoomInAction=new QAction(QIcon(":/images/zoomin.png"),tr("zoom In"),this);
	zoomInAction->setStatusTip("zoom in");
	connect(zoomInAction,SIGNAL(triggered()),glWidget,SLOT(slotZoomIn()));

	zoomOutAction=new QAction(QIcon(":/images/zoomout.png"),tr("zoom out"),this);
	zoomInAction->setStatusTip("zoom out");
	connect(zoomOutAction,SIGNAL(triggered()),glWidget,SLOT(slotZoomOut()));
}  

void MainUI::createMenus()  
{  
    fileMenu = menuBar()->addMenu(tr("File"));  
    fileMenu->addAction(fileNewAction);  
    fileMenu->addAction(fileOpenAction);  
    fileMenu->addAction(fileSaveAction);  
    fileMenu->addAction(exitAction);  
  
  
    editMenu = menuBar()->addMenu(tr("Edit"));  
    editMenu->addAction(copyAction);  
    editMenu->addAction(cutAction);  
    editMenu->addAction(pasteAction);  
  
  
    aboutMenu = menuBar()->addMenu(tr("Help"));  
    aboutMenu->addAction(aboutAction);  
  
}  
  
void MainUI::createToolBars()  
{  
    fileTool = addToolBar("File");  
    fileTool->addAction(fileNewAction);  
    fileTool->addAction(fileOpenAction);  
    fileTool->addAction(fileSaveAction);  
  
    editTool = addToolBar("Edit");  
    editTool->addAction(copyAction);  
    editTool->addAction(cutAction);  
    editTool->addAction(pasteAction);  

	directionTool=addToolBar("Direction");
	directionTool->addAction(leftAction);
	directionTool->addAction(rightAction);
	directionTool->addAction(upAction);
	directionTool->addAction(downAction);
	
	rotateTool=addToolBar("Rotate");
	rotateTool->addAction(xPAction);
	rotateTool->addAction(xNAction);
	rotateTool->addAction(yPAction);
	rotateTool->addAction(yNAction);
	rotateTool->addAction(zPAction);
	rotateTool->addAction(zNAction);
	
	zoomTool=addToolBar("zoom");
	zoomTool->addAction(zoomInAction);
	zoomTool->addAction(zoomOutAction);
}  
  
void MainUI::slotNewFile()  
{  
   
}

void MainUI::slotOpenFile()  
{  
    fileName = QFileDialog::getOpenFileName(this,"open Obj flie dialog","/","OBJ files (*.obj)");  
    if(!fileName.isEmpty())  
    {  
      // loadFile(fileName) ;
		emit  signal_loadObj( fileName);
    }  
  
}  

void MainUI::loadFile(QString fileName)  
{  
  
} 

void MainUI::slotSaveFile()
{

}
