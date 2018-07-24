#pragma once
#include "scene.h"
#include "EulerAngles.h"
const int maximumIterations = 1;



 
	
class IK : public Scene
{
	//int linksNum = 4;

protected:
	glm::vec3 tipPosition;
	std::vector<glm::vec3> jointPositions;
	std::vector<int> jointDistances;
	float distanceBetweenJoints;
	float distanceRootToTagret;
	bool isIKactive;
	bool reachable;
	
    float maxDistance; 
	std::vector<glm::vec3> newPositions;
	std::vector< EulerAngles> euler;

public:
	enum objKind { snake, fruit, obj, shotingObj, killObj,bullet, shotObj};
	glm::vec3 destPosition;
	bool boxMode;
	bool cameraMode;
	double delta;
	IK(void);
	virtual ~IK(void);
	IK(glm::vec3 position,float angle,float hwRelation,float near, float far);
	
	void init(Vertex *vertices,unsigned int *indices,int verticesSize,int indicesSize,bool cameraMode);

	void addShape(int type, int parent);
	void addShape(const std::string& fileName, int parent);
	void addShape(const std::string& fileName,const std::string& textureFileName, int parent,int kind);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName, int parent,int kind);
	void addShape(int CylParts,int linkPosition,int parent);
	void addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent,int kind);

	void inline changeMode(){cameraMode = !cameraMode;}
	void makeChange();
	void calculateStep(bool EulerVersion);
	float Distance(int indx1, int indx2);



	// funcs

	void selectBox();
	void selectCamera();
	void updateData();
	void getDistances();
	void printData();
	void IKrotate(int i, float angle,float phi);
	void IKNewPosTrans();

	// Inline funcs

	bool inline isActive() const { return isIKactive; }
	void inline deactivateIK() { isIKactive = false; }
	void inline activateIK() { isIKactive = true; }
	void inline nextLink() { 
		boxMode = false;  
		cameraMode = false;  
		pickedShape = (pickedShape + 1) % (getShapesSize() - 1); 
	}
	void inline prevLink() { 
		boxMode = false;  
		cameraMode = false; 
		pickedShape = ((getShapesSize() - 1) + pickedShape - 1) % (getShapesSize() - 1); 
	}
	void setLinkNum(int numOflinks);

};

