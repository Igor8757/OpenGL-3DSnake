#pragma once
#include "MovableGLM.h"
#include "mesh.h"
#include "lineMesh.h"
#include "texture.h"
#include <list>
#include "kdtree.h"
#include <time.h>

class Shape : public MovableGLM
{
	LineMesh *lineMesh;
	Texture *tex;
	bool isCopy;
	std::vector<glm::vec2> rotVectors;
public:
	Mesh * mesh;
	glm::vec2 getRotVector();
	inline int GerRotVecSize() { return rotVectors.size(); }
	void addRotVector(glm::vec2 addVector);
	bool colladind = false;
	void makeKDTree(const IndexedModel& model);
	Kdtree kdtree;
	bool _isCollidingA(Node & nodeA, Node & nodeB, Shape & other, Node & current);
	//bool _isCollidingA(Node & nodeA, Node & nodeB);
	//bool _isCollidingA(Node& nodeA, Node& nodeB, Node& currentA, Node& currentB, Shape& other);
	bool _isCollidingB(Node& nodeA, Node& nodeB);
	bool _isColliding(BoundingBox& A, BoundingBox& B, Shape &other);
	bool _isCollidingB(Node & nodeA, Node & nodeBNode, Node &current, Shape & other);


	char* name = "";
	bool getIfCokkading();
	Shape GetShpe();
	Node* getNode();
	Shape *shape;
	Node *node;

	int linkNumber;
	bool isItem;
	bool isSnake = false;
	int collisionRecDepth = 0;
	glm::mat4 snakeLinkPosition = glm::mat4(1);

	enum{triangles,lines};
	Shape(const Shape& shape);
	Shape(const std::string& fileName);
	Shape(const std::string& fileName,const std::string& textureFileName);
	Shape(int CylParts, int linkPosition);
	Shape(int CylParts, int linkPosition,const std::string& textureFileName);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string& textureFileName);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void draw(int mode);
	void getPoints(std::list<glm::vec3> &pList);
	bool isColliding(Shape& other);
	virtual ~Shape(void);

	void setItem(bool b) {isItem = b;	}
};

