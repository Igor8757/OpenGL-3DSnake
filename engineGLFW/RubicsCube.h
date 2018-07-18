#pragma once
#include "scene.h"
	const int squarsNum = 3;
class RubicsCube : public Scene
{
private:

	std::vector<int> indxArray;
	int direction;
	int factor;
	int rotCount;
	int lastAxis;
public:
	RubicsCube(void);
	RubicsCube(vec3 position,float angle,float hwRelation,float near, float far);

	void init(Vertex *vertices,unsigned int *indices,int verticesSize,int indicesSize);

	void addShape(int type);
	void addShape(const std::string& fileName);
	void addShape(const std::string& fileName,const std::string& textureFileName);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName);

	void inline changeDirection(){direction = direction * -1;}
	void inline scaleFactor(float f) {if(rotCount%factor == 0)
										if(f>1 || (factor>=1 && f<1)) 
											factor = factor * f;}
	void rotateWall(int type,float amt);
	void updateIndices(int type);
	virtual ~RubicsCube(void);
};

