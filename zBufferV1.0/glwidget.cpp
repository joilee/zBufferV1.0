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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //glClear()������������Ƕ�initializeGL()����
                                                        //�����õ���ɫ�ͻ�����ȵ�������
    glLoadIdentity();//���õ�ǰ��ģ�͹۲����,�þ�ִ����󣬽������ƶ�������Ļ������


	if(!model.polygons.empty())
	{
		// ʹ��ɨ����z-buffer�㷨����ģ��
		zbuffer.zpaint();
		
	}
	//glutSwapBuffers();


//    /*���漸������������������ε�,��glBegin()��ʼ��glEnd()������glVertex3fΪ����һ��3ά�Ķ��㣬����ֵΪ������*/
//    //  glTranslatef(-1.5f, 0.0f, -6.0f);//�����ƶ���ǰ����λ��
//    glTranslatef(-0.3, 0.3, -0.6);
//    glColor3f(1.0f, 0.0f, 0.0f);
//  //  glDisable(GL_TEXTURE_2D);
//    glBegin(GL_TRIANGLES);//GL_TRIANGLES����Ϊ��������
//    glVertex3f(0.0f, 0.2f, 0.0f);//�϶�������
//    glVertex3f(-0.2f, -0.2f, 0.0f);//���½�����
//    glVertex3f(0.2f, -0.2f, 0.0f);//���½�����
//    glEnd();//��������
//
//    glLoadIdentity();//���½��㶨λ��ͬ������Ļ������
//    glTranslatef(0.3f,0.3f,0.0f); // ��x���������ƶ�0.3����λ
//    glColor3f(0.0f,1.0f,0.0f);//��ɫ���÷�������ط���������Ķ������ö�����Ч��
//
//    /*���濪ʼ�����ı���*/
//    glBegin(GL_QUADS);
//    glVertex3f(-0.2f, 0.2f, 0.0f); // ���϶���
//    glVertex3f( 0.2f, 0.2f, 0.0f); // ���϶���
//    glVertex3f( 0.2f, -0.2f, 0.0f); // ���¶���
//    glVertex3f(-0.2f, -0.2f, 0.0f); // ���¶���
//    glEnd(); // �ı��λ��ƽ���
//
//    glLoadIdentity();
//    glTranslatef(0.0f, -0.3f, 0.0f);
//    glColor3f(0.0f, 0.0f, 1.0f);
//
//    /*�������ĵ����ƽ�Բ�����ǻ���һ��Բ��*/
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
//    /*������������������������ƽ�Բ�����ʵ��Բ�ܵĻ���*/
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
        height = 1;//��ֹһ����Ϊ0
    glViewport(0, 0, (GLint)width, (GLint)height);//���õ�ǰ�ӿڣ������������ô��ڵģ�ֻ���������ﱻQt����װ����
    glMatrixMode(GL_PROJECTION);//ѡ��ͶӰ����
    glLoadIdentity();//����ѡ��õ�ͶӰ����
   // gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);//����͸��ͶӰ����
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 }

 void slot_receiveObj(QString path)
 {
 //���������

 //���ļ�����


 }