#include "mainui.h"

MainUI::MainUI(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//ui.setupUi(this);
	this->resize( QSize( 800, 600 ));
	setWindowTitle(tr("QMainWindow"));  
    //text = new QTextEdit(this); 
	glWidget =new GLWidget();
    setCentralWidget(glWidget);  
  
    createActions();  
    createMenus();  
    createToolBars();  
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
    connect(cutAction,SIGNAL(triggered()),text,SLOT(cut()));  
  
    copyAction = new QAction(QIcon(":/images/copy.png"),tr("Copy"),this);  
    copyAction->setShortcut(tr("Ctrl+C"));  
    copyAction->setStatusTip(tr("copy to clipboard"));  
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));  
  
    pasteAction = new QAction(QIcon(":/images/paste.png"),tr("paste"),this);  
    pasteAction->setShortcut(tr("Ctrl+V"));  
    pasteAction->setStatusTip(tr("paste clipboard to selection"));  
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));  
  
    aboutAction = new QAction(tr("About"),this);  
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(slotAbout()));  
  
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
}  
  
void MainUI::slotNewFile()  
{  
    MainUI *newWin = new MainUI;  
    newWin->show();  
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
  //  printf("file name:%s\n",fileName.data());  
    QFile file(fileName);  
    if(file.open(QIODevice::ReadOnly))  
    {  
		if(glWidget->glmObjModel != NULL)
		{
			glmDelete(glWidget->glmObjModel);
		}
		std::string str = fileName.toStdString();

		char* ch =new char(strlen(str.c_str())+1);
		memset(ch,'\0',strlen(str.c_str())+1);
		strcpy(ch,str.c_str());
		glWidget->glmObjModel = glmReadOBJ(ch);//read obj file by using GLM
		glWidget->model.toZufferModel(glWidget->glmObjModel);//translate to zBufferModel 
		glWidget->zbuffer.setModel(&glWidget->model);//完成读取和设置
    }  
} 

void MainUI::slotSaveFile()
{}