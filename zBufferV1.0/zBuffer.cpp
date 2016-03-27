#include "zBuffer.h"
#include <algorithm>
#include <functional>
#include <math.h>
#include "dataManage.h"
#include <QDebug>

#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h> 

zBuffer::zBuffer()
{

}

zBuffer::~zBuffer(void)
{
	pt->clear();
	et->clear();
	apt.clear();
}


void zBuffer::setModel(zBufferModel* model)
{
	this->model = model;
}

void zBuffer::clear()
{
	for(int i = 0; i <= HEIGHT_MAX; i++)
	{
		pt[i].clear();
		et[i].clear();
	}
}

void zBuffer::genratePTandET()
{
	clear();

	int ymax,ymin,ymax_edge;
	int i;
	size_t id_pre,id0,id1;//record vertex id of polygon
	int polygonNum = static_cast<int>(model->polygons.size());

	vector<size_t>::iterator it,it_end;

	//initialize the size of image,minimum and maximum of y
	img_top = 0;
	img_bottom = HEIGHT_MAX;

	for(i = 0;i < polygonNum;i++)
	{
		it = model->polygons[i].ver_id.begin();
		it_end = model->polygons[i].ver_id.end();

		id0 = id_pre = *it;//the first id of vertices
		it++;
		ymax = model->vertices_rendering[id_pre].y;
		ymin = model->vertices_rendering[id_pre].y;

		for(;it != it_end;it++)
		{
			Edge edge(model->vertices_rendering[id_pre],model->vertices_rendering[*it],ymax_edge,i);
			et[toInt(ymax_edge)].push_back(edge);//store edge into 边表

			id1 = id_pre;
			id_pre = *it;
			 
			//calculate the min and max of y
			if(ymax < model->vertices_rendering[id_pre].y)
				ymax = model->vertices_rendering[id_pre].y;
			else if(ymin > model->vertices_rendering[id_pre].y)
				ymin = model->vertices_rendering[id_pre].y;

		}
		//the line between the last vertex and the first vertex
		Edge edge(model->vertices_rendering[id_pre],model->vertices_rendering[id0],ymax_edge,i);
		et[toInt(ymax_edge)].push_back(edge);//store edge into 边表
		
		//construct 多边形表
		//now id0:the first vertex id           id1:the middle vertex id id_pre:the last vertex id
		PolygonStruct polygon(model->vertices_rendering[id0],model->vertices_rendering[id1],model->vertices_rendering[id_pre]);
		polygon.IP = i;
		polygon.dy = ymax - ymin + 1;
		
		pt[toInt(ymax)].push_back(polygon);//store polygon into 多边形表

		if(img_top < toInt(ymax)) img_top = ymax;
		if(img_bottom > toInt(ymin)) img_bottom = ymin;
	}
}

void zBuffer::zpaint()
{
	if (!model->changed)
	{
		glDrawPixels(WIDTH_MAX,HEIGHT_MAX,GL_RGB,GL_UNSIGNED_BYTE,fbuffer_L);
		return;
	}

	//产生多边形表和边表
	genratePTandET();
	
	int i,j,k,draw_count=0;
	int currentX;
	float currentZ;
	float a_c,b_c,d_c;

	ActivePolygon* aPolygon;
	ActiveEdge *aEdgeL,*aEdgeR;
	Edge *edge;

	//clear image buffer
	memset(&fbuffer_L,0,sizeof(fbuffer_L));

	//set light parameters
	Vertex eye(0,0,1); //set eye normal
	Vertex lightL(255,255,255);
	Vertex lightE(255,255,255);
	Vertex lightD(0,20,50); //set light dot normal

	lightD.Unitise();

	double Ka = 0.3,Kd = 0.25,Ks = 0.45; //set light constant

	for(i = img_top;i >= img_bottom;i--)
	{
		//put ymax of polygon == 1 into apt
		for(j = static_cast<int>(pt[i].size()) - 1;j >= 0;j --)
		{
			apt.push_back(pt[i][j]);//store polygon into apt
		}

		//foreach active polygon
		for(j = static_cast<int>(apt.size()) - 1;j >= 0;j--)
		{
			aPolygon = &(apt[j]);
			if(aPolygon->dy <= 0)
			{
				aPolygon->aet.clear();
				apt.erase(apt.begin() + j);
				continue;
			}
			
			//delete 已经结束的活化边
			for(k = static_cast<int>(aPolygon->aet.size()) - 1;k >= 0;k--)
			{
				if(aPolygon->aet[k].dyl <= 0)
				{
					aPolygon->aet.erase(aPolygon->aet.begin() + k);
				}
			}
			//预先计算出下面循环中要用的几个变量
			a_c = -(aPolygon->a / aPolygon->c);//-a/c
			b_c = -(aPolygon->b / aPolygon->c);//-b/c
			d_c = -(aPolygon->d / aPolygon->c);//-d/c

			//加入新的活化边
			for(k = static_cast<int>(et[i].size()) - 1; k >= 0;k--)
			{
				edge = &(et[i][k]);
				if(edge->IP == aPolygon->IP && edge->dy > 0)
				{
					ActiveEdge aEdge;
					aEdge.xl = edge->x;
					aEdge.dyl = edge->dy;

					if(abs(aPolygon->c) < 1e-6)//c=0,与z-axis垂直
					{
						aEdge.zl = aPolygon->d;
						aEdge.dzx = 0;
						aEdge.dzy = 0;
					}
					else
					{
						aEdge.zl = a_c * edge->x + b_c * i + d_c;//根据平面方程得出
						aEdge.dzx = a_c;
						aEdge.dzy = -b_c;
					}
					
					if(abs(edge->dy) < 1e-6)//斜率为0.平行于x-axis，做特殊处理
					{
						aEdge.dxl = 0;
					}
					else
					{
						aEdge.dxl = edge->dx;
					}
					
					aPolygon->aet.push_back(aEdge);
				}
			}

			//sort aet
			sort(aPolygon->aet.begin(),aPolygon->aet.end(),greater<ActiveEdge>());

		}

		//绘制当前扫描行

		//设置背景色和z-buffer初值
		for(j = 0;j < WIDTH_MAX;j++)
		{
			zbuffer[j] = FLT_MIN;
		}
		for(j = static_cast<int>(apt.size())-1;j >= 0;j--)
		{
			aPolygon = &(apt[j]);
			for(k = static_cast<int>(aPolygon->aet.size()) - 1;k >= 0;k -= 2)
			{
				aEdgeL = &(aPolygon->aet[k]);
				aEdgeR = &(aPolygon->aet[k-1]);

				currentX = aEdgeL->xl;
				currentZ = aEdgeL->zl;
				for(;currentX <= aEdgeR->xl;currentX++,currentZ += aEdgeL->dzx)
				{
					//当前的z值比保存值大，绘制当前点
					if(currentZ > zbuffer[currentX])
					{
						zbuffer[currentX] = currentZ;
						//fbuffer_L[i][current_x] = current_z *1.3;//设置颜色
						int ip = aPolygon->IP;
						Vertex N(model->polygons[ip].fnormal.x, model->polygons[ip].fnormal.y, model->polygons[ip].fnormal.z);
						N.Unitise();
						Vertex color;
						color = lightE * Ka + lightL * Kd * (lightD * N) + lightL * Ks * pow((eye +lightD) / 2 * N,10);
						if(color.x > 255)
							color.x = 255;
						if(color.y > 255)
							color.y = 255;
						if(color.z > 255)
							color.z = 255;
						if (color.x < 0)
							color.x = 0;
						if (color.y < 0)
							color.y = 0;
						if (color.z < 0)
							color.z = 0;
						
						fbuffer_L[i][currentX][0] = color.x;
						fbuffer_L[i][currentX][1] = color.y;
						fbuffer_L[i][currentX][2] = color.z;
						 
						draw_count++;

					}//颜色设置完毕
				}//当前活化边对扫描完毕
				//更新活化边
				aEdgeL->dyl --;
				aEdgeR->dyl --;
				aEdgeL->xl += aEdgeL->dxl;
				aEdgeR->xl += aEdgeR->dxl;
				aEdgeL->zl += aEdgeL->dzx * aEdgeL->dxl + aEdgeL->dzy;
				aEdgeR->zl += aEdgeR->dzx * aEdgeR->dxl + aEdgeR->dzy;
			}
			//更新活化多边形
			aPolygon->dy --;
		}
	}

	qDebug("Pixels:%d\n",draw_count);

	//clear active polygon and active edge
	for(i = static_cast<int>(apt.size())-1;i >= 0;i--)
	{
		apt[i].aet.clear();
	}

	apt.clear();
	model->changed = false;
	//render the whole window
	glDrawPixels(WIDTH_MAX,HEIGHT_MAX,GL_RGB,GL_UNSIGNED_BYTE,fbuffer_L);

}
