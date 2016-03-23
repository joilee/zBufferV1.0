#ifndef DATAMANAGE_H
#define DATAMANAGE_H

#define FLT_MAX 3.402823466e+38F
#define FLT_MIN 1.175494351e-38F

#define WIDTH_MAX 512
#define HEIGHT_MAX 512 

#include <math.h>
#include <vector>
#include "glm.h"

using namespace std;

const int PI = 3.14159265;

static int WIN_WIDTH,WIN_HEIGHT;
static float scale_global;
static int rot[3];
static float trans[3];

template<class Tclass>
inline Tclass maxValue(Tclass a, Tclass b, Tclass c){
	if (a > b) {
		return a > c ? a : c;
	} else {
		return b > c ? b : c;
	}
}

inline int toInt(float a){
	return (int)(a + 0.5);
}

//Vertex
class Vertex
{
public:
	float x,y,z,w;

	Vertex();
	Vertex(float xPara,float yPara,float zPara);
	Vertex(float xPara,float yPara,float zPara,float wPara);
	
	//operation
	float dot(Vertex* p2);
	Vertex cross(Vertex* p2);

	//vector unitised
	void Unitise();

	inline Vertex operator + (Vertex t) //addition
	{
		return Vertex(x + t.x,y + t.y,z + t.z);
	}
	inline Vertex operator - (Vertex t) //subtraction
	{
		return Vertex(x - t.x,y - t.y,z - t.z);
	}
	inline Vertex operator * (float t) //dot product
	{
		return Vertex(x * t,y * t,z * t);
	}
	inline float operator * (Vertex t) //scalar product
	{
		return x * t.x + y * t.y + z * t.z;
	}
	inline Vertex operator / (float t) //devide product
	{
		return Vertex(x/t,y/t,z/t);
	}


};

//edge
typedef struct _Edge
{
	int x; // �ߵ��϶˵��x����
	float dx; //��������ɨ���߽����x�����
	int dy; //�߿�Խ��ɨ���ߵ���Ŀ
	int IP; //����������εı��

	_Edge(){}
	_Edge(Vertex p1,Vertex p2,int &ymax,int ip)
	{
		init(&p1,&p2,ymax);
		IP = ip;
	}
	inline void init(Vertex* p1,Vertex* p2,int &ymax)
	{
		if (p1->y > p2->y)
		{
			ymax = toInt(p1->y);
			x = toInt(p1->x);
			dy = ymax - toInt(p2->y);
			dx = (dy == 0 ? (p1->x - p2->x) : ((p2->x - p1->x) / (p1->y - p2->y)));
		}
		else {
			ymax = toInt(p2->y);
			x = toInt(p2->x);
			dy = ymax - toInt(p1->y);
			dx = (dy == 0 ? (p2->x - p1->x) : ((p2->x - p1->x) / (p1->y - p2->y)));
		}
	}
}Edge;

//active edge
typedef struct _ActiveEdge
{
	float xl;	// �󽻵��x����
	float dxl;	// (�󽻵����)������ɨ���߽����x����֮��
	int dyl;	// �Ժ��󽻵����ڱ��ཻ��ɨ������Ϊ��ֵ, �Ժ�����ÿ����һ��ɨ���߼�1
	float xr;
	float dxr;
	int dyr;
	float zl;	// �󽻵㴦���������ƽ������ֵ
	float dzx;	// ��ɨ���������߹�һ������ʱ, ���������ƽ����������. ����ƽ�淽��, dzx = -a/c (c!= 0)
	float dzy;	// ��y���������ƹ�һ��ɨ����ʱ, ���������ƽ����������. ����ƽ�淽��, dzy = b/c (c!= 0)
	int IP;		// ����������εı��

	_ActiveEdge(){}
	_ActiveEdge(int ip):IP(ip)
	{
		xl = FLT_MAX;
	}

	bool operator < (const _ActiveEdge& rhs) const {
		return xl < rhs.xl;
	}
	bool operator > (const _ActiveEdge& rhs) const {
		return xl > rhs.xl;
	}

}ActiveEdge;

//polygon and active polygon
typedef struct _Polygon{
	_Polygon(int ip):IP(ip){}
	float a,b,c,d;	// ��������ڵ�ƽ��ķ���ϵ�� ax + by + cz + d = 0,(a,b,c)Ϊ�淨���ٴ���һ����ɵ�d
	int IP;			// ����εı��
	int dy;			// ����ο�Խ��ɨ������Ŀ
	//Color color;	// ����ε���ɫ
	vector<ActiveEdge> aet;	//���ýṹΪ�����νڵ�ʱ����Ӧ�ö���εĻ�߱�

	_Polygon(Vertex p1, Vertex p2, Vertex p3){
		a = (p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y);
		b = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
		c = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
		d = -(a * p1.x + b * p1.y + c * p1.z);
		float tmp = sqrt(a*a+b*b+c*c+d*d);
		if (tmp)
		{
			a /= tmp;
			b /= tmp;
			c /= tmp;
			d /= tmp;
		}
		if(c==0) 
			d = p1.z;//��ֱ��z��ʱ����d�洢zֵ
	}
	_Polygon(Vertex* p1, Vertex* p2, Vertex* p3){
		init(p1, p2, p3);
	}
	inline void init(Vertex* p1, Vertex* p2, Vertex* p3)
	{
		a = (p2->y - p1->y) * (p3->z - p1->z) - (p2->z - p1->z) * (p3->y - p1->y);
		b = (p2->z - p1->z) * (p3->x - p1->x) - (p2->x - p1->x) * (p3->z - p1->z);
		c = (p2->x - p1->x) * (p3->y - p1->y) - (p2->y - p1->y) * (p3->x - p1->x);
		d = -(a * p1->x + b * p1->y + c * p1->z);
		float tmp = sqrt(a*a+b*b+c*c+d*d);
		a /= tmp;
		b /= tmp;
		c /= tmp;
		d /= tmp;
	}
}PolygonStruct,ActivePolygon;


//polygon coming from the obj file
typedef struct _RawPolygon
{
	vector<size_t> ver_id;//the id of vertices of the polygon
	Vertex fnormal;//the normal of the polygon
}RawPolygon;

//obj Model
class zBufferModel
{
public:
	//original vertex data
	vector<Vertex> vertices_original;

	//rendering vertex data,which can be changed such as panning,rotating,zooming
	vector<Vertex> vertices_rendering;

	//polygon data coming from the obj file
	vector<RawPolygon> polygons;

	//identify whether the model changed,true presents changed and need to redraw
	bool changed;
	Vertex dim_max;
	Vertex dim_min;

	zBufferModel();
	~zBufferModel(void);

	//the maximum of x,y,z
	void getModelDimension();
	
	//translate the model point to screen pixel coordinate
	void normalize()
	{
		float s,trans;
		trans = WIN_WIDTH < WIN_HEIGHT ? WIN_WIDTH : WIN_HEIGHT;
		getModelDimension();
		s = trans / (2.5 * maxValue(dim_max.x - dim_min.x,dim_max.y - dim_min.y, dim_max.z - dim_min.z));
		trans /= 2;

		for (int i = static_cast<int>(vertices_original.size()-1);i >= 0;i--)
		{
			vertices_original[i].x = vertices_original[i].x * s;
			vertices_original[i].y = vertices_original[i].y * s;
			vertices_original[i].z = vertices_original[i].z * s;
			vertices_rendering[i].x = toInt(vertices_original[i].x + trans);
			vertices_rendering[i].y = toInt(vertices_original[i].y + trans);
			vertices_rendering[i].z = toInt(vertices_original[i].z + trans);

		}

		getModelDimension();
		changed = true;

		scale_global = 1;
		rot[0] = rot[1] = rot[2] = 0;

	}

	//translate glm model to zbuffer model 
	void toZufferModel(GLMmodel * glmModel)
	{
		unsigned int i;
		vertices_rendering.clear();
		polygons.clear();
		for(i = 1;i <= glmModel->numvertices;i++)
		{
			Vertex pt(glmModel->vertices[3 * i + 0], glmModel->vertices[3 * i + 1], glmModel->vertices[3 * i + 2], 0);
			vertices_original.push_back(pt);
			vertices_rendering.push_back(pt);
		}
		for(i = 0;i < glmModel->numtriangles;i++)
		{
			RawPolygon raw;
			raw.ver_id.push_back(glmModel->triangles[i].vindices[0]-1);
			raw.ver_id.push_back(glmModel->triangles[i].vindices[1] - 1);
			raw.ver_id.push_back(glmModel->triangles[i].vindices[2] - 1);

			polygons.push_back(raw);
		}
		if (glmModel->numnormals == 0)
		{
			glmFacetNormals(glmModel);
			for (i = 0;i < glmModel->numtriangles;i++)
			{
				Vertex fnormal(glmModel->facetnorms[i],glmModel->facetnorms[i+1],glmModel->facetnorms[i+2]);
				polygons[i].fnormal = fnormal;
			}
		}
		else
		{
			for (i = 0;i < glmModel->numtriangles;i++)
			{
				Vertex fnormal(0,0,0);
				int nindex;
				for(int j = 0;j < 3;j++)
				{
					nindex = glmModel->triangles[i].nindices[j];
					fnormal.x += glmModel->normals[nindex * 3 + 0];
					fnormal.y += glmModel->normals[nindex * 3 + 1];
					fnormal.z += glmModel->normals[nindex * 3 + 2];

				}
				polygons[i].fnormal = fnormal;
			}
		}
		glmDelete(glmModel);
		normalize();
		printf("%d vertices\n%d polygons\n",vertices_rendering.size(),polygons.size());
	}

};

#endif