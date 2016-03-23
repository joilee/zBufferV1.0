#include <QtGui>
#include <QtOpenGL>
#include <GL/glut.h>
 #include <math.h>
 #include "dataManage.h"
 #include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#define GL_PI 3.1415926
#define GL_RADIUX  0.2f

 GLWidget::GLWidget(QGLWidget *parent)
 { 
	 glmObjModel=NULL;
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


	if(!model.polygons.empty())
	{
		// 使用扫描线z-buffer算法绘制模型
		zbuffer.zpaint();
		
	}
	//glutSwapBuffers();


//    /*下面几句代码是用来画三角形的,以glBegin()开始，glEnd()结束；glVertex3f为给定一个3维的顶点，坐标值为浮点型*/
//    //  glTranslatef(-1.5f, 0.0f, -6.0f);//首先移动当前焦点位置
//    glTranslatef(-0.3, 0.3, -0.6);
//    glColor3f(1.0f, 0.0f, 0.0f);
//  //  glDisable(GL_TEXTURE_2D);
//    glBegin(GL_TRIANGLES);//GL_TRIANGLES代表为画三角形
//    glVertex3f(0.0f, 0.2f, 0.0f);//上顶点坐标
//    glVertex3f(-0.2f, -0.2f, 0.0f);//左下角坐标
//    glVertex3f(0.2f, -0.2f, 0.0f);//右下角坐标
//    glEnd();//结束画完
//
//    glLoadIdentity();//重新焦点定位，同样是屏幕的中心
//    glTranslatef(0.3f,0.3f,0.0f); // 向x轴正方向移动0.3个单位
//    glColor3f(0.0f,1.0f,0.0f);//颜色设置放在这个地方，对下面的顶点设置都是有效的
//
//    /*下面开始绘制四边形*/
//    glBegin(GL_QUADS);
//    glVertex3f(-0.2f, 0.2f, 0.0f); // 左上顶点
//    glVertex3f( 0.2f, 0.2f, 0.0f); // 右上顶点
//    glVertex3f( 0.2f, -0.2f, 0.0f); // 右下顶点
//    glVertex3f(-0.2f, -0.2f, 0.0f); // 左下顶点
//    glEnd(); // 四边形绘制结束
//
//    glLoadIdentity();
//    glTranslatef(0.0f, -0.3f, 0.0f);
//    glColor3f(0.0f, 0.0f, 1.0f);
//
//    /*用连续的点来逼近圆这里是画的一个圆周*/
////    GLfloat x, y, angle;
////    glBegin(GL_POINTS);
////    for(angle = 0; angle <2*GL_PI; angle +=0.001)
////    {
////        x = GL_RADIUX*sin(angle);
////        y = GL_RADIUX*cos(angle);
////        glVertex3d(x, y, 0.0);
////    }
////    glEnd();
////    glPopMatrix();
//
//    /*这里用连续的三角形面积来逼近圆的面积实现圆周的画法*/
//    GLint circle_points = 100, i = 0;
//    glBegin(GL_TRIANGLE_FAN);
//    for(int i = 0; i < circle_points; i++ )
//    {
//        double angle = 2*GL_PI*i/circle_points;
//        glVertex3d(0.2*cos(angle), 0.2*sin(angle), 0);
//    }
//    glEnd();
 }

 void GLWidget::resizeGL(int width, int height)
 {
	 if(0 == height)
        height = 1;//防止一条边为0
    glViewport(0, 0, (GLint)width, (GLint)height);//重置当前视口，本身不是重置窗口的，只不过是这里被Qt给封装好了
    glMatrixMode(GL_PROJECTION);//选择投影矩阵
    glLoadIdentity();//重置选择好的投影矩阵
   // gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);//建立透视投影矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 }

 void slot_receiveObj(QString path)
 {
 //将数据清空

 //将文件读入


 }