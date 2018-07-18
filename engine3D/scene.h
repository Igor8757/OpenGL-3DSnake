#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include <vector>

class Scene : public MovableGLM
{
	
	
protected:
	std::vector<int> chainParents;
	int pickedShape;
	static const int scaleFactor = 3;
	std::vector<Shader*> shaders;
	std::vector<Shape*> shapes;
	Shape *axisMesh;
	int Links;
	int linksNum = 4;
	std::vector<Camera*> cameras; //light will have the properties of camera
	std::vector<glm::vec3> rotPositions;
	std::vector<clock_t> TimeOfRotiains;
public:
	inline void addRotTime(clock_t curr_time) { TimeOfRotiains.push_back(curr_time); }
	inline void addRotPositions(glm::vec3 rotPosition) { rotPositions.push_back(rotPosition); }
	inline int GetLinkNum() { return linksNum; }
	void move();
	void inline setPicked(int pickID) { pickedShape = pickID; }
	void addVectorToShapes(glm::vec2 addVector);
	bool checkIftimeToMove(int shapeIdx);
	std::vector<glm::mat4> shapesNormal;
	enum axis{xAxis,yAxis,zAxis};
	enum transformations{xLocalTranslate,yLocalTranslate,zLocalTranslate,xGlobalTranslate,yGlobalTranslate,zGlobalTranslate,
		xLocalRotate,yLocalRotate,zLocalRotate,xGlobalRotate,yGlobalRotate,zGlobalRotate,xScale,yScale,zScale,xCameraTranslate,yCameraTranslate,zCameraTranslate};
	Scene();
	Scene(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void addShape(int type,int parent);
	void addShape(const std::string& fileName,int parent);
	void addShape(const std::string& fileName,const std::string& textureFileName,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName,int parent);
	void addShape(int Cyparts,int linkPosition,int parent);
	void addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent);
	void addTerrain(const std::string & fileName, const std::string & textureFileName);

	void addShader(const std::string& fileName);

	glm::mat4 GetViewProjection(int indx) const;
	glm::mat4 GetShapeTransformation() const;
	void draw(int shaderIndx,int cameraIndx,bool drawAxis);
	void shapeTransformation(int type,float amt);
	void shapeRotation(glm::vec3 v, float ang,int indx);
	//void inline setPicked(int pickID){pickedShape = pickID;}
	float picking(double x,double y);
	void resize(int width,int hight,int near,int far);
	//void updateTipPosition(int indx);
	glm::vec3 getTipPosition(int indx);
	glm::vec3 getDestination(int indx);
	glm::vec3 getAxisDirection(int indx,int axis);
	inline void setParent(int indx,int newValue) {chainParents[indx]=newValue;}
	virtual ~Scene(void);

	int inline getPickedShape() { return pickedShape; }
	int inline getShapesSize() { return shapes.size(); }
	Camera * getCamera() { return cameras.at(0); }

	bool checkCollision(int shape1,int shape2);

	float xMovement = 0;
	float yMovement = 0;
	float zMovement = 0;

};

