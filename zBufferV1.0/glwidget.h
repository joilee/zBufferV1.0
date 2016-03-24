#ifndef GLWIDGET_H

#define GLWIDGET_H

 

#include <QtOpenGL/QtOpenGL>

#include <QWidget>
#include "glm.h"
#include "dataManage.h"
#include "zBuffer.h"

class GLWidget : public QGLWidget

{

  Q_OBJECT

public:
	GLMmodel* glmObjModel;
	zBufferModel model;
	zBuffer zbuffer;
  GLWidget(QGLWidget *parent = 0);
  ~GLWidget();
   void translate(zBufferModel *model, int x, int y, int z);
    void rotate(zBufferModel *model, int axis, int angle) ;
    void scale(zBufferModel *model, float scale_factor);
  public slots:
	  void slot_receiveObj(QString path);
	  void slotLeft();
	  void slotRight();
	  void slotUp();
	  void slotDown();
	  void slotRotateXP();
	  void slotRotateXN();
	  void slotRotateYP();
	  void slotRotateYN();
      void slotRotateZP();
	  void slotRotateZN();
	  void slotZoomIn();
	  void slotZoomOut();

protected:
  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );

private:

  

};


#endif