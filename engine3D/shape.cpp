#include "shape.h"

Shape::Shape(const Shape& shape)
{
	if(shape.mesh)
		mesh = shape.mesh;
	if(shape.lineMesh)
		lineMesh = shape.lineMesh;
	tex = shape.tex;
	isCopy = true;
}
Shape::Shape(const std::string& fileName){
	mesh = new Mesh(fileName);
	tex = 0;
	isCopy = false;

}

Shape::Shape(const std::string& fileName,const std::string& textureFileName,int kind){
	
	mesh = new Mesh(fileName);
	tex = new Texture(textureFileName);
	isCopy = false;
	this->kind = kind;
}

Shape::Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices);
	tex = 0; 
	isCopy = false;

}

Shape::Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{

	lineMesh = new LineMesh(vertices,numVertices,indices,numIndices);
	tex = 0; 
	isCopy = false;

}


Shape::Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName,int kind)
{
	this->kind = kind;
	mesh = new Mesh(vertices,numVertices,indices,numIndices);
	tex = new Texture(textureFileName);
	isCopy = false;

}

Shape::Shape(int CylParts,int linkPosition)
{
	mesh = new Mesh(CylParts,linkPosition);
	tex = 0; 
	isCopy = false;

}

Shape::Shape(int CylParts,int linkPosition,const std::string& textureFileName, int kind)
{
	this->kind = kind;
	mesh = new Mesh(CylParts,linkPosition);
	tex = new Texture(textureFileName); 
	isCopy = false;

}
glm::vec2 Shape::getRotVector() {
	glm::vec2 tempVec = rotVectors.at(0);
	/*rotVectors.reserve(rotVectors.size());
	rotVectors.pop_back();
	rotVectors.reserve(rotVectors.size());*/
	rotVectors.erase(rotVectors.begin());
	return tempVec;

}
void Shape::addRotVector(glm::vec2 addVector) {
	rotVectors.push_back(addVector);

}
void Shape::addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices);
	tex = new Texture(textureFileName);
}

void Shape::addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices);
}

void Shape::addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	lineMesh = new LineMesh(vertices,numVertices,indices,numIndices);
}

void Shape::draw(int mode)
{
	//transformations
	if(tex)
		tex->Bind();
	if(mesh)
		mesh->Draw(mode);
	if(lineMesh)
		lineMesh->Draw();
}

Shape::~Shape(void)
{
	if(!isCopy)
	{
		if(mesh)
 			delete mesh;
		if(lineMesh)
			delete lineMesh;
		if(tex)
			delete tex;
	}
}


void Shape::getPoints(std::list<glm::vec3> &pList)
{/*
	
	if (tipPosition != glm::vec3(0))
		A.P = tipPosition;
	else
		A.P = glm::vec3(A.mat[3][0], A.mat[3][1], A.mat[3][2]);

	if (other.tipPosition != glm::vec3(0))
		B.P = other.tipPosition;
	else
		B.P = glm::vec3(B.mat[3][0], B.mat[3][1], B.mat[3][2]);

	glm::vec3 T = B.P - A.P;
	glm::vec3 L;

	A.uX = findAxis(A.uX);
	A.uY = findAxis(A.uY);
	A.uZ = findAxis(A.uZ);
	B.uX = findAxis(B.uX);
	B.uY = findAxis(B.uY);
	B.uZ = findAxis(B.uZ);
	*/
}


bool Shape::_isColliding(BoundingBox& A, BoundingBox& B, Shape& other) {
	A.mat = A.mat*makeTransScale(glm::mat4(1));
	A.P = glm::vec3(A.mat[3][0], A.mat[3][1], A.mat[3][2]);
	B.mat = B.mat*other.makeTransScale(glm::mat4(1));
	B.P = glm::vec3(B.mat[3][0], B.mat[3][1], B.mat[3][2]);

	if (isSnake)
	{
		A.P = glm::vec3(snakeLinkPosition[3][0], snakeLinkPosition[3][1], snakeLinkPosition[3][2]);
	}
	else
		A.P = glm::vec3(A.mat[3][0], A.mat[3][1], A.mat[3][2]);


	glm::vec3 T = B.P - A.P;
	glm::vec3 L;

	A.uX = glm::vec3(1, 0, 0);
	A.uY = glm::vec3(0, 1, 0);
	A.uZ = glm::vec3(0, 0, 1);
	B.uX = glm::vec3(1, 0, 0);;
	B.uY = glm::vec3(0, 1, 0);;
	B.uZ = glm::vec3(0, 0, 1);;
	float Rxx = glm::dot(A.uX, B.uX);
	float Rxy = glm::dot(A.uX, B.uY);
	float Rxz = glm::dot(A.uX, B.uZ);
	float Ryx = glm::dot(A.uY, B.uX);
	float Ryy = glm::dot(A.uY, B.uY);
	float Ryz = glm::dot(A.uY, B.uZ);
	float Rzx = glm::dot(A.uZ, B.uX);
	float Rzy = glm::dot(A.uZ, B.uY);
	float Rzz = glm::dot(A.uZ, B.uZ);

	// case 1 : L = Ax
	if (std::abs(glm::dot(T, A.uX)) >
		A.W + std::abs(B.W * Rxx) + std::abs(B.H * Rxy) + std::abs(B.D * Rxz)) {
		return false;
	}
	// case 2 : L = Ay

	if (std::abs(glm::dot(T, A.uY)) >
		A.H + std::abs(B.W * Ryx) + std::abs(B.H * Ryy) + std::abs(B.D * Ryz)) {
		return false;
	}
	// case 3 : L = Az

	if (std::abs(glm::dot(T, A.uZ)) >
		A.D + std::abs(B.W * Rzx) + std::abs(B.H * Rzy) + std::abs(B.D * Rzz)) {
		return false;
	}
	// case 4 : L = Bx

	if (std::abs(glm::dot(T, B.uX)) >
		B.W + std::abs(A.W * Rxx) + std::abs(A.H * Rxy) + std::abs(A.D * Rzx)) {
		return false;
	}
	// case 5 : L = By

	if (std::abs(glm::dot(T, B.uY)) >
		B.H + std::abs(A.W * Rxy) + std::abs(A.H * Ryy) + std::abs(A.D * Rzy)) {
		return false;
	}
	// case 6 : L = Bz

	if (std::abs(glm::dot(T, B.uZ)) >
		B.D + std::abs(A.W * Rxz) + std::abs(A.H * Ryz) + std::abs(A.D * Rzz)) {
		return false;
	}
	// case 7 : L = Ax x Bx

	if (std::abs(glm::dot(T, A.uZ)*Ryx - glm::dot(T, A.uY)*Rzx) >
		std::abs(A.H * Rzx) + std::abs(A.D * Ryx) + std::abs(B.H * Rxz) + std::abs(B.D * Rxy)) {
		return false;
	}
	// case 8 : L = Ax x By

	if (std::abs(glm::dot(T, A.uZ)*Ryy - glm::dot(T, A.uY)*Rzy) >
		std::abs(A.H * Rzy) + std::abs(A.D * Ryy) + std::abs(B.W * Rxz) + std::abs(B.D * Rxx)) {
		return false;
	}

	// case 9 : L = Ax x Bz

	if (std::abs(glm::dot(T, A.uZ)*Ryz - glm::dot(T, A.uY)*Rzz) >
		std::abs(A.H * Rzz) + std::abs(A.D * Ryz) + std::abs(B.W * Rxy) + std::abs(B.H * Rxx)) {
		return false;
	}

	// case 10 : L = Ay x Bx

	if (std::abs(glm::dot(T, A.uX)*Rzx - glm::dot(T, A.uZ)*Rxx) >
		std::abs(A.W * Rzx) + std::abs(A.D * Rxx) + std::abs(B.H * Ryz) + std::abs(B.D * Ryy)) {
		return false;
	}

	// case 11 : L = Ay x By

	if (std::abs(glm::dot(T, A.uX)*Rzy - glm::dot(T, A.uZ)*Rxy) >
		std::abs(A.W * Rzy) + std::abs(A.D * Rxy) + std::abs(B.W * Ryz) + std::abs(B.D * Ryx)) {
		return false;
	}

	// case 12 : L = Ay x Bz

	if (std::abs(glm::dot(T, A.uX)*Rzz - glm::dot(T, A.uZ)*Rxz) >
		std::abs(A.W * Rzz) + std::abs(A.D * Rxz) + std::abs(B.W * Ryy) + std::abs(B.H * Ryx)) {
		return false;
	}

	// case 13 : L = Az x Bx

	if (std::abs(glm::dot(T, A.uY)*Rxx - glm::dot(T, A.uX)*Ryx) >
		std::abs(A.W * Ryx) + std::abs(A.H * Rxx) + std::abs(B.H * Rzz) + std::abs(B.D * Rzy)) {
		return false;
	}

	// case 14 : L = Az x By

	if (std::abs(glm::dot(T, A.uY)*Rxy - glm::dot(T, A.uX)*Ryy) >
		std::abs(A.W * Ryy) + std::abs(A.H * Rxy) + std::abs(B.W * Rzz) + std::abs(B.D * Rzx)) {
		return false;
	}

	// case 15 : L = Az x Bz

	if (std::abs(glm::dot(T, A.uY)*Rxz - glm::dot(T, A.uX)*Ryz) >
		std::abs(A.W * Ryz) + std::abs(A.H * Rxz) + std::abs(B.W * Rzy) + std::abs(B.H * Rzx)) {
		return false;
	}


	collisionRecDepth++;
	return true;
}

bool Shape::getIfCokkading()
{
	return colladind;
}
/*
Shape Shape::GetShpe()
{
return Shape(node->data.vertices, sizeof(node->data.vertices), node->data.indices, sizeof(node->data.indices));
}
*/
Node * Shape::getNode()
{
	return node;
}



void Shape::makeKDTree(const IndexedModel& model) {
	std::list<BoundingBox> posList;

	for (int i = 0; i < model.positions.size(); i++) {
		glm::vec3 vertex1 = model.positions.at(i); 
		//glm::vec3 vertex = getVectorInSystem(glm::mat4(1), vertex1);
		glm::vec3 vertex = vertex1 * (scaleFactor * 0.87f);
		BoundingBox b;
		b.position = vertex;
		posList.push_back(b);
	}
	kdtree.makeTree(posList);
	node = kdtree.getRoot();
	//Shape  shape1(kdtree.getRoot()->data.vertices, sizeof( kdtree.getRoot()->data.vertices), kdtree.getRoot()->data.indices,sizeof(kdtree.getRoot()->data.indices));
	//shape = &shape1;
}
bool Shape::_isCollidingA(Node& nodeA, Node& nodeB, Shape &other, Node &current) {
	if (&nodeA == nullptr) {

		return true;
	}
	if (collisionRecDepth >= 3)
	{
		return true;
	}

	BoundingBox A = nodeA.data;
	BoundingBox B = nodeB.data;
	//node = &currentA;

	if (_isColliding(A, B, other)) {
		/*
		if (_isCollidingA(*nodeA.left, *nodeB.left, nodeA, nodeB, other))
		{
		return true;
		}
		else if (_isCollidingA(*nodeA.left, *nodeB.right, nodeA, nodeB, other)) {
		return true;
		}
		else if (_isCollidingA(*nodeA.right, *nodeB.left, nodeA, nodeB, other)) {
		return true;
		}
		else if (_isCollidingA(*nodeA.right, *nodeB.right, nodeA, nodeB, other)) {
		return true;
		}
		*/
		
		if (_isCollidingB(current, *(other.kdtree.getRoot()), *(other.kdtree.getRoot()), other)) {
			//node = &current;
			if (_isCollidingA(*nodeA.left, nodeB, other, nodeA))
			{

				return true;
			}
			if (_isCollidingA(*nodeA.right, nodeB, other, nodeA)) {
				return true;
			}

		}
	}
	colladind = false;
	return false;

}
bool Shape::_isCollidingB(Node& nodeA, Node& nodeB, Node &current, Shape& other) {
	if (&nodeB == nullptr) {

		return true;
	}
	
	BoundingBox A = nodeA.data;
	BoundingBox B = nodeB.data;
	//other.node = &current;
	//other.node = &current;
	if (_isColliding(A, B, other)) {

		if (_isCollidingB(nodeA, *nodeB.left, nodeB, other)) {
			if (nodeB.left == nullptr) {
				other.node = &current;
			}
			return true;
		}
		if (_isCollidingB(nodeA, *nodeB.right, nodeB, other)) {
			if (nodeB.right == nullptr) {
				other.node = &current;
			}
			return true;
		}



		colladind = false;
		//Shape  shape1(node->data.vertices, sizeof(node->data.vertices), node->data.indices, sizeof(node->data.indices));
		//shape = &shape1;
	}
	
	return false;

}

bool Shape::isColliding(Shape& other) {
	collisionRecDepth = 0;
	return _isCollidingA(*(kdtree.getRoot()), *(other.kdtree.getRoot()), other, *(kdtree.getRoot()));

}
Shape& Shape::operator=(const Shape &other) {
	translateMat[0] = other.translateMat[0];
	translateMat[1] = other.translateMat[1];
	//setTraslateMat(other.getTraslateMat());
	linkNumber = other.linkNumber;
	int i=0;
	for (i; i < rotVectors.size(); i++) {
		rotVectors[i] = other.rotVectors[i];
	}

	//setRotVectors(other.getRotVectors());
	mesh = other.mesh;
	return *this;
}
