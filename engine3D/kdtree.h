/*
*
* k-d tree declaration file
*
* author: Arvind Rao
* license: GNU
*
*
*/

#ifndef KDTREE_
#define KDTREE_
#define _USE_MATH_DEFINES

#include <memory>
#include <vector>
#include <list>
#include <cmath>
#include <queue>
#include <utility>
#include <glm\glm.hpp>
#include <mesh.h>
#include <Eigen\Dense>

const int N = 3;

class BoundingBox {
public:
	glm::vec3 position;
	float max = std::numeric_limits<float>::max();
	float min = std::numeric_limits<float>::min();
	float minX = max, minY = max, minZ = max;
	float maxX = min, maxY = min, maxZ = min;

	glm::vec3 xyz, Xyz, xYz, xyZ, XYz, XyZ, xYZ, XYZ;

	glm::vec3 P;

	float H, W, D;

	glm::vec3 uX, uY, uZ;

	Eigen::MatrixXf S;

	Eigen::MatrixXf U;

	void SVD();

	glm::mat4 mat;

	glm::mat4 rotmat = glm::mat4(1);


	Vertex vertices[24] =
	{
		Vertex(glm::vec3(-0.1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),  //blue wall 
		Vertex(glm::vec3(-0.1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(0.1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(0.1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-0.1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),  // g&b wall 
		Vertex(glm::vec3(-0.1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(0.1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(0.1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-0.1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(1, 1, 0)),  // r&g wall 
		Vertex(glm::vec3(-0.1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(0.1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(0.1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-0.1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(0, 1, 0)),  // green wall 
		Vertex(glm::vec3(-0.1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(0.1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(0.1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-0.1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),  // red wall 
		Vertex(glm::vec3(-0.1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-0.1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-0.1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(0.1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),  // r&b wall 
		Vertex(glm::vec3(0.1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(0.1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(0.1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
	};


	unsigned int indices[36] = { 0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20
	};

	void setBoundingBox(std::list<BoundingBox>& plist, bool bonus, Eigen::MatrixXf scatter);

	void setUnitVectors();

	void setVertices(float maxX, float minX, float maxY, float minY, float maxZ, float minZ);

	void setVerticesSVD(float maxX, float minX, float maxY, float minY, float maxZ, float minZ);


	unsigned int* makeIndices();

	glm::vec3 findCenter(std::list<BoundingBox>& plist);

};

class Node
{
public:
	typedef glm::vec4 vecType;
	//typedef BoundingBox vecType:
	Node* left;// = std::unique_ptr<Node>( new Node(3) );
	Node* right; //( new Node(3));
	BoundingBox data;

	//default constructor
	Node(int _n);

	//copy constructor
	Node(int _n, BoundingBox _data);

	//default deconstructor
	~Node();


};

class Kdtree
{

public:
	//typedef float                numType;
	//typedef Node::vecType vecType;

	//default constructor
	Kdtree();

	//default deconstructor
	~Kdtree();

	/*
	*   Return the tree root node
	*/
	Node* getRoot() const { return root; };
	/*
	* support function for printTree
	*/
	void print_data(BoundingBox pt);

	/*  prints the tree
	*   and really works best for small trees
	*   as a test of tree construction.
	*/
	void printTree(Node* head);

	void makeTree(std::list<BoundingBox>& plist);

	//function for finding the median of a list of points
	BoundingBox findMedian(int axis, std::list<BoundingBox> &plist, std::list<BoundingBox> &left,
		std::list<BoundingBox> &right, glm::vec3 units[]);
	//function for making the tree


	void _makeTree(Node * head, std::list<BoundingBox>& plist, int depth);


	Eigen::MatrixXf findScatter(std::list<BoundingBox>& plist, glm::vec3 centerOfMass);

	glm::vec3 findCenterOfMass(std::list<BoundingBox>& plist);

	bool bonus = false;

private:
	glm::vec3 findCenter(std::list<BoundingBox>& plist);
	//helper for makeTree 
	void _makeTree(Node* head, std::list<BoundingBox> &plist, int depth, float maxX, float minX, float maxY, float minY, float maxZ, float minZ);

	Node* root;
};


#endif