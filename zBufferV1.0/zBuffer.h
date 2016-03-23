#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "dataManage.h"
#include <GL/glut.h>
#include <vector>

class zBuffer
{
public:
	zBuffer();
	~zBuffer(void);

	void setModel(zBufferModel * model);

	void genratePTandET();

	void zpaint();

private:
	int img_top,img_bottom;
	float zbuffer[WIDTH_MAX];
	GLubyte fbuffer_L[HEIGHT_MAX][WIDTH_MAX][3];//color buffer
	zBufferModel * model;
	
	vector<Edge> et[HEIGHT_MAX+1];//边y桶
	vector<ActivePolygon> apt;//活化多边形表
	vector<PolygonStruct> pt[HEIGHT_MAX+1];//多边形y桶

	void clear();
	


};

#endif