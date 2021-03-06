#include <QtGui>
#include <QtOpenGL>
#include <GL/glut.h>
 #include <math.h>
 #include "dataManage.h"
 #include "glwidget.h"
#include "time.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#define GL_PI 3.1415926
#define GL_RADIUX  0.2f

 GLWidget::GLWidget(QGLWidget *parent)
 { 
	glmObjModel = NULL;
 }
 
GLWidget::~GLWidget()
 {
 }

void GLWidget::initializeGL()
 {
	glShadeModel(GL_SMOOTH);  
    glClearColor( 0.0, 0.0, 0.0, 0.0 );  
    glClearDepth( 1.0 );  
    glEnable( GL_DEPTH_TEST );  
    glDepthFunc( GL_LEQUAL );  
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );  
  
    return;   
 }


 void GLWidget::paintGL()
 {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //glClear()函数在这里就是对initializeGL()函数
                                                        //中设置的颜色和缓存深度等起作用
    glLoadIdentity();//重置当前的模型观察矩阵,该句执行完后，将焦点移动到了屏幕的中心

	clock_t start = clock();
	if(!model.polygons.empty())
	{
		// 使用扫描线z-buffer算法绘制模型
		zbuffer.zpaint();

	}
	clock_t end = clock();
	printf("Time Spend:%ld ms \n",end - start);
	
	//glutSwapBuffers();
 }

 void GLWidget::resizeGL(int width, int height)
 {
	 if(0 == height)
        height = 1;//防止一条边为0

	 WIN_WIDTH = width;
	 WIN_HEIGHT = height;
    glViewport(0, 0, (GLint)width, (GLint)height);//重置当前视口，本身不是重置窗口的，只不过是这里被Qt给封装好了
    glMatrixMode(GL_PROJECTION);//选择投影矩阵
    glLoadIdentity();//重置选择好的投影矩阵
   // gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);//建立透视投影矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 }

 void GLWidget::slot_receiveObj(QString path)
 {
	 

  //清空数据

 //将文件读入
	 QFile file(path);
	  if(file.open(QIODevice::ReadOnly))  
	  {
		  /*if(glmObjModel != NULL)
		  {
			  glmDelete(glmObjModel);
		  }*/
		  std::string str = path.toStdString();

		  char* ch =new char(strlen(str.c_str())+1);
		  memset(ch,'\0',strlen(str.c_str())+1);
		  strcpy(ch,str.c_str());
		  glmObjModel = glmReadOBJ(ch);//read obj file by using GLM
		  model.toZufferModel(glmObjModel);//translate to zBufferModel 
		  zbuffer.setModel(&model);//完成读取和设置
	//	  QMessageBox::information(0,QObject::tr("Success"),QObject::tr("File loaded！"));
	  } else
	  {
		  QMessageBox::information(0,QObject::tr("错误"),QObject::tr("未能成功加载文件"));
	  }
	  
		updateGL();
 }

 void GLWidget::translate(zBufferModel *model, int x, int y, int z){
	 if(	(model->dim_max.x + x > WIDTH_MAX) || (model->dim_max.y + y > HEIGHT_MAX)|| 
		 (model->dim_min.x + x < 0) || (model->dim_min.y + y < 0) )
	 {
		 return;
	 }	

	 for (int i = static_cast<int>(model->vertices_original.size() - 1); i >= 0; i--)
	 {
		 model->vertices_rendering[i].x += x;
		 model->vertices_rendering[i].y += y;
		 model->vertices_rendering[i].z += z;
	 }
	 model->changed = true;
 }

 void GLWidget::rotate(zBufferModel *model, int axis, int angle) {
	 int i,tmp1,tmp2;
	 float sinA,cosA,radian;
	 float trans;
	 trans = (WIN_WIDTH < WIN_HEIGHT ? WIN_WIDTH : WIN_HEIGHT) / 2;
	 radian = angle * PI / 180.f;//将角度转换为弧度
	 sinA = sin(radian);
	 cosA = cos(radian);
	 switch(axis){
	 case 1://x轴
		 for (i = static_cast<int>(model->vertices_original.size() - 1); i >= 0; i--)
		 {
			 tmp1 = model->vertices_rendering[i].y - trans;
			 tmp2 = model->vertices_rendering[i].z - trans;
			 model->vertices_rendering[i].y = toInt(tmp1 * cosA -tmp2 * sinA + trans);
			 model->vertices_rendering[i].z = toInt(tmp1 * sinA + tmp2 * cosA + trans);
		 }
		 break;
	 case 2://y轴
		 for (i = static_cast<int>(model->vertices_original.size() - 1); i >= 0; i--)
		 {
			 tmp1 = model->vertices_rendering[i].z- trans;
			 tmp2 = model->vertices_rendering[i].x- trans;
			 model->vertices_rendering[i].z = toInt((tmp1 * cosA - tmp2 * sinA) * scale_global + trans);
			 model->vertices_rendering[i].x = toInt((tmp1 * sinA + tmp2 * cosA) * scale_global + trans);
		 }
		 break;
	 case 3://z轴
		 for (i = static_cast<int>(model->vertices_original.size() - 1); i >= 0; i--)
		 {
			 tmp1 = model->vertices_rendering[i].x - trans;
			 tmp2 = model->vertices_rendering[i].y - trans;
			 model->vertices_rendering[i].x = toInt(tmp1 * cosA - tmp2 * sinA + trans);
			 model->vertices_rendering[i].y = toInt(tmp1 * sinA + tmp2 * cosA + trans);
		 }			
		 break;
	 }
	 model->changed = true;
 }

 void GLWidget::scale(zBufferModel *model, float scale_factor){
	 float trans;
	 trans = WIN_WIDTH < WIN_HEIGHT ? WIN_WIDTH : WIN_HEIGHT;
	 trans /= 2;
	 //printf("scale_global = %f, scale_factor = %f, a*b=%f\n",scale_global,scale_factor,scale_global * scale_global);
	 scale_global *= scale_factor;
	 if(scale_global * scale_factor < 0.5 || 
		 (model->dim_max.x - trans) * scale_global + trans > WIDTH_MAX || 
		 (model->dim_max.y - trans) * scale_global + trans > HEIGHT_MAX|| 
		 (model->dim_min.x - trans) * scale_global + trans < 0 || 
		 (model->dim_min.y - trans) * scale_global + trans < 0)
	 {
		 scale_global /= scale_factor;
		 return;
	 }	

	 for (int i = static_cast<int>(model->vertices_original.size() - 1); i >= 0; i--)
	 {
		 model->vertices_rendering[i].x = toInt((model->vertices_rendering[i].x - trans) * scale_factor + trans);
		 model->vertices_rendering[i].y = toInt((model->vertices_rendering[i].y - trans)  * scale_factor + trans);
		 model->vertices_rendering[i].z = toInt((model->vertices_rendering[i].z - trans)  * scale_factor + trans);
	 }
	 model->changed = true;
 }

 void GLWidget::slotRight()
 {
	 translate(&model, 3, 0, 0);
	 updateGL();
 }

 void GLWidget::slotUp()
 {
	 translate(&model, 0, 3, 0);
	 updateGL();
 }
 void GLWidget::slotLeft()
 {
	 translate(&model, -3, 0, 0);
	 updateGL();
 }
 void GLWidget::slotDown()
 {
	 translate(&model, 0, -3, 0);
	 updateGL();
 }
 void GLWidget::slotRotateXN()
 {
	rotate(&model,1,-5);
	updateGL();
 }
 void GLWidget::slotRotateXP()
 {
	 rotate(&model,1,5);
	 updateGL();
 }
 void GLWidget::slotRotateYP()
 {
	 rotate(&model,2, 5);
	 updateGL();
 }
 void GLWidget::slotRotateYN()
 {
	 rotate(&model,2, -5);
	 updateGL();
 }
  void GLWidget::slotRotateZP()
 {
	 rotate(&model,3,10);
	 updateGL();
 }
  void GLWidget::slotRotateZN()
  {
	  rotate(&model,3,-10);
	  updateGL();
  }

  void GLWidget::slotZoomIn()
  {
	  scale(&model,1.1);
	  updateGL();
  }
  void GLWidget::slotZoomOut()
  {
	  scale(&model,1/1.1);
	  updateGL();
  }
