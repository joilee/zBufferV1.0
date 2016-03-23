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

  public slots:
	  void slot_receiveObj(QString path);
	  void slot_finishLoadObj();

protected:
  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );

private:

  

};


#endif