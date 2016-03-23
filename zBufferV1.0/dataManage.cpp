#include <math.h>
#include <vector>
#include "glm.h"
#include "dataManage.h"

using namespace std;

//------------------------------------Vertex---------------------------------------------------------
Vertex::Vertex()
{

}
Vertex::Vertex(float xPara,float yPara,float zPara):x(xPara),y(yPara),z(zPara),w(0){}
Vertex::Vertex(float xPara,float yPara,float zPara,float wPara):x(xPara),y(yPara),z(zPara),w(wPara){}

//operation
float Vertex::dot(Vertex* p2)
{
   return x * p2->x + y * p2->y + z * p2->z + w * p2->w;
}
Vertex Vertex::cross(Vertex* p2)
{
	Vertex p;
	p.x = y * p2->z - z * p2->y;
	p.y = z * p2->x - x * p2->z;
	p.z = x * p2->y - y * p2->x;
	return p;
}

//vector unitised
void Vertex::Unitise()
{
	float len = sqrt(dot(this));
	if(len == 0.0)
	{
		x = 0;
		y = 0;
		z = 0;
	}
	else
	{
		x /= len;
		y /= len;
		z /= len;
	}
}

//-------------------------------------zBufferModel---------------------------------------------
zBufferModel::zBufferModel()
{

}
zBufferModel::~zBufferModel(void)
{
	vertices_original.clear();
	vertices_rendering.clear();
	polygons.clear();
}


void zBufferModel::getModelDimension()
{
	dim_max.x = dim_max.y = dim_max.z = FLT_MIN;
	dim_min.x = dim_min.y = dim_min.z = FLT_MAX;

	for (int i = static_cast<int>(vertices_rendering.size() - 1);i >= 0;i--)
	{
		if (vertices_rendering[i].x > dim_max.x)
		{
			dim_max.x = vertices_rendering[i].x;
		}
		else if(vertices_rendering[i].x < dim_min.x)
		{
			dim_min.x = vertices_rendering[i].x;
		}
		if (vertices_rendering[i].y > dim_max.y)
		{
			dim_max.y = vertices_rendering[i].y;
		}
		else if(vertices_rendering[i].y < dim_min.y)
		{
			dim_min.y = vertices_rendering[i].y;
		}
		if (vertices_rendering[i].z > dim_max.z)
		{
			dim_max.z = vertices_rendering[i].z;
		}
		else if(vertices_rendering[i].z < dim_min.z)
		{
			dim_min.z = vertices_rendering[i].z;
		}

	}
}

//void zBufferModel::normalize()
//{
//	float s,trans;
//	trans = WIN_WIDTH < WIN_HEIGHT ? WIN_WIDTH : WIN_HEIGHT;
//	this->getModelDimension();
//	s = trans / (2.5 * maxValue(dim_max.x - dim_min.x,dim_max.y - dim_min.y, dim_max.z - dim_min.z));
//	trans /= 2;
//
//	for (int i = static_cast<int>(vertices_original.size()-1);i >= 0;i--)
//	{
//		vertices_original[i].x = vertices_original[i].x * s;
//		vertices_original[i].y = vertices_original[i].y * s;
//		vertices_original[i].z = vertices_original[i].z * s;
//		vertices_rendering[i].x = toInt(vertices_original[i].x + trans);
//		vertices_rendering[i].y = toInt(vertices_original[i].y + trans);
//		vertices_rendering[i].z = toInt(vertices_original[i].z + trans);
//
//	}
//
//	this->getModelDimension();
//	changed = true;
//	printf("model normalized...\n");
//
//	scale_global = 1;
//	rot[0] = rot[1] = rot[2] = 0;
//}

//void zBufferModel::toZufferModel(GLMmodel * glmModel)
//{
//	unsigned int i;
//	vertices_rendering.clear();
//	polygons.clear();
//	for(i = 1;i <= glmModel->numvertices;i++)
//	{
//		Vertex pt(glmModel->vertices[3 * i + 0], glmModel->vertices[3 * i + 1], glmModel->vertices[3 * i + 2], 0);
//		vertices_original.push_back(pt);
//		vertices_rendering.push_back(pt);
//	}
//	for(i = 0;i < glmModel->numtriangles;i++)
//	{
//		RawPolygon raw;
//		raw.ver_id.push_back(glmModel->triangles[i].vindices[0]-1);
//		raw.ver_id.push_back(glmModel->triangles[i].vindices[1] - 1);
//		raw.ver_id.push_back(glmModel->triangles[i].vindices[2] - 1);
//
//		polygons.push_back(raw);
//	}
//	if (glmModel->numnormals == 0)
//	{
//		glmFacetNormals(glmModel);
//		for (i = 0;i < glmModel->numtriangles;i++)
//		{
//			Vertex fnormal(glmModel->facetnorms[i],glmModel->facetnorms[i+1],glmModel->facetnorms[i+2]);
//			polygons[i].fnormal = fnormal;
//		}
//	}
//	else
//	{
//		for (i = 0;i < glmModel->numtriangles;i++)
//		{
//			Vertex fnormal(0,0,0);
//			int nindex;
//			for(int j = 0;j < 3;j++)
//			{
//				nindex = glmModel->triangles[i].nindices[j];
//				fnormal.x += glmModel->normals[nindex * 3 + 0];
//				fnormal.y += glmModel->normals[nindex * 3 + 1];
//				fnormal.z += glmModel->normals[nindex * 3 + 2];
//
//			}
//			polygons[i].fnormal = fnormal;
//		}
//	}
//	glmDelete(glmModel);
//	normalize();
//	printf("Read %d vertices...\nRead %d polygons\n",vertices_rendering.size(),polygons.size());
//}
//
