/**
*
* k-d tree implementation file
*
* author: Arvind Rao
* license: GNU
*
*
*/
#include "kdtree.h"
#include <iostream>


// implementation of BoundingBox Class

void BoundingBox::setBoundingBox(std::list<BoundingBox>& plist, bool bonus, Eigen::MatrixXf scatter) {
	if (plist.size()>1) {
		P = findCenter(plist);
		mat = glm::mat4(1);
		mat[3] = glm::vec4(P, 1);
		H = (maxY - minY) / 2;
		W = (maxX - minX) / 2;
		D = (maxZ - minZ) / 2;
		setVertices(maxX, minX, maxY, minY, maxZ, minZ);
		//indices = makeIndices();
		if (bonus) {
			S = scatter;
			SVD();
		}
		else {
			rotmat = glm::mat4(1);
		}
		setUnitVectors();
	}
	else {
		P = plist.front().position;
		H = 0;
		W = 0;
		D = 0;
		maxX = 0; minX = 0; maxY = 0; minY = 0; maxZ = 0; minZ = 0;
		mat = glm::mat4(1);
		mat[3] = glm::vec4(P, 1);
		uX = glm::vec3(1, 0, 0);
		uY = glm::vec3(0, 1, 0);
		uZ = glm::vec3(0, 0, 1);
	}

}

void BoundingBox::setUnitVectors() {
	/*
	if (Xyz - xyz != glm::vec3(0))
		uX = glm::normalize(Xyz - xyz);
	else
		uX = glm::vec3(1, 0, 0);
	if (xYz - xyz != glm::vec3(0,0,0))
		uY = glm::normalize(xYz - xyz);
	else
		uY = glm::vec3(0, 1, 0);
	if (xyZ - xyz != glm::vec3(0))
		uZ = glm::normalize(xyZ - xyz);
	else
		uZ = glm::vec3(0, 0, 1);
	*/
	uX = glm::vec3(1, 0, 0);
	uY = glm::vec3(0, 1, 0);
	uZ = glm::vec3(0, 0, 1);
}



// implementation of Node Class

Node::Node(int _n) { left = 0; right = 0; }
Node::Node(int _n, BoundingBox _data) : data(_data) { left = 0; right = 0; }

Node::~Node() {}


// implementation of kdtree

Kdtree::Kdtree() {}
Kdtree::~Kdtree() {}

/*
*
*
*/
BoundingBox Kdtree::findMedian(int axis, std::list<BoundingBox> &plist, std::list<BoundingBox> &left,
	std::list<BoundingBox> &right, glm::vec3 units[])
{
	glm::mat4 rotmat2;
	if (0) {
		rotmat2 = glm::mat4(glm::vec4(units[0], 0), glm::vec4(units[1], 0), glm::vec4(units[2], 0), glm::vec4(0, 0, 0, 1));
		/*glm::mat4 trans = glm::mat4(1);
		trans[0] = rotmat * glm::vec4(1, 0, 0, 1);
		trans[1] = rotmat * glm::vec4(0, 1, 0, 1);
		trans[2] = rotmat * glm::vec4(0, 0, 1, 1);*/
		for (auto it = plist.begin(); it != plist.end(); it++) {
			glm::vec4 v4(it->position, 1);
			v4 = v4 * glm::transpose(rotmat2);
			it->position.x = v4.x;
			it->position.y = v4.y;
			it->position.z = v4.z;
		}
	}

	BoundingBox median;
	int size = plist.size();
	int med = ceil(float(size) / float(2));
	int count = 0;

	if (size == 1)
		return plist.front();

	// Using lambda function here, to define comparison function--parametrized by 'axis'
	plist.sort([&](BoundingBox& a, BoundingBox& b) {return a.position[axis] < b.position[axis]; });

	for (auto& x : plist)
	{
		if (count < med)
			left.push_back(x);
		else
			right.push_back(x);
		++count;
	}
	median = left.back();
	left.pop_back();
	return median;
}

void Kdtree::print_data(BoundingBox pt)
{
	for (int i = 0; i < N; i++)
	{
		std::cout << pt.position[i] << ", ";
	}
	std::cout << "\n";
}

/*
*
*
*/
void Kdtree::printTree(Node* head)
{
	//find the tree depth 
	int maxdepth = 3;
	int spaces = pow(2, maxdepth + 1) - 1;
	int depth = 0;

	std::cout << "\n**** Print of Tree **********\n";
	std::queue< Node* > current, next;
	Node * temp = head;
	current.push(temp);

	while (!current.empty())
	{
		temp = current.front();
		current.pop();

		if (temp == nullptr)
			std::cout << "NULL\n";
		else
		{
			Kdtree::print_data(temp->data);
			next.push(temp->left);
			next.push(temp->right);
		}
		if (current.empty())
		{
			depth++;
			std::cout << "level: " << depth << "\n";
			std::swap(current, next);
		}
	}
}

/*
*  algorithm is based on http://en.wikipedia.org/wiki/Kd_tree
*/
void Kdtree::makeTree(std::list<BoundingBox>& plist)
{
	Node* head = new Node(3);
	Kdtree::_makeTree(head, plist, 0);
	Kdtree::root = head;
}

void BoundingBox::setVertices(float maxX, float minX, float maxY, float minY, float maxZ, float minZ)
{

	vertices[0] = Vertex(glm::vec3(minX, minY, minZ), glm::vec2(1, 0), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));/* blue wall */
	vertices[1] = Vertex(glm::vec3(minX, maxY, minZ), glm::vec2(0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));
	vertices[2] = Vertex(glm::vec3(maxX, maxY, minZ), glm::vec2(0, 1), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));
	vertices[3] = Vertex(glm::vec3(maxX, minY, minZ), glm::vec2(1, 1), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));
	;
	vertices[4] = Vertex(glm::vec3(minX, minY, maxZ), glm::vec2(1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1));/* g&b wall */
	vertices[5] = Vertex(glm::vec3(minX, maxY, maxZ), glm::vec2(0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1));
	vertices[6] = Vertex(glm::vec3(maxX, maxY, maxZ), glm::vec2(0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1));
	vertices[7] = Vertex(glm::vec3(maxX, minZ, maxZ), glm::vec2(1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1));
	;
	vertices[8] = Vertex(glm::vec3(minX, minY, minZ), glm::vec2(0, 1), glm::vec3(0, -1, 0), glm::vec3(1, 1, 0));/* r&g wall */
	vertices[9] = Vertex(glm::vec3(minX, minY, maxZ), glm::vec2(1, 1), glm::vec3(0, -1, 0), glm::vec3(1, 1, 0));
	vertices[10] = Vertex(glm::vec3(maxX, minY, maxZ), glm::vec2(1, 0), glm::vec3(0, -1, 0), glm::vec3(1, 1, 0));
	vertices[11] = Vertex(glm::vec3(maxX, minY, minZ), glm::vec2(0, 0), glm::vec3(0, -1, 0), glm::vec3(1, 1, 0));
	;
	vertices[12] = Vertex(glm::vec3(minX, maxY, minZ), glm::vec2(0, 1), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));/* green wall */
	vertices[13] = Vertex(glm::vec3(minX, maxY, maxZ), glm::vec2(1, 1), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	vertices[14] = Vertex(glm::vec3(maxX, maxY, maxZ), glm::vec2(1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	vertices[15] = Vertex(glm::vec3(maxX, maxY, minZ), glm::vec2(0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	;
	vertices[16] = Vertex(glm::vec3(minX, minY, minZ), glm::vec2(1, 1), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)); /* red wall */
	vertices[17] = Vertex(glm::vec3(minX, minY, maxZ), glm::vec2(1, 0), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0));
	vertices[18] = Vertex(glm::vec3(minX, maxY, maxZ), glm::vec2(0, 0), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0));
	vertices[19] = Vertex(glm::vec3(minX, maxY, minZ), glm::vec2(0, 1), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0));
	;
	vertices[20] = Vertex(glm::vec3(maxX, minY, minZ), glm::vec2(1, 1), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));/* r&b wall */
	vertices[21] = Vertex(glm::vec3(maxX, minY, maxZ), glm::vec2(1, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));
	vertices[22] = Vertex(glm::vec3(maxX, maxY, maxZ), glm::vec2(0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));
	vertices[23] = Vertex(glm::vec3(maxX, maxY, minZ), glm::vec2(0, 1), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));


}

void BoundingBox::setVerticesSVD(float maxX, float minX, float maxY, float minY, float maxZ, float minZ)
{
	glm::mat4 rotmat(glm::vec4(uX, 0), glm::vec4(uY, 0), glm::vec4(uZ, 0), glm::vec4(0, 0, 0, 1));
	vertices[0] = Vertex(glm::vec3(minX, minY, minZ), glm::vec2(1, 0), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));/* blue wall */
	vertices[1] = Vertex(glm::vec3(minX, maxY, minZ), glm::vec2(0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));
	vertices[2] = Vertex(glm::vec3(maxX, maxY, minZ), glm::vec2(0, 1), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));
	vertices[3] = Vertex(glm::vec3(maxX, minY, minZ), glm::vec2(1, 1), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));
	vertices[4] = Vertex(glm::vec3(minX, minY, maxZ), glm::vec2(1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1));/* g&b wall */
	vertices[5] = Vertex(glm::vec3(minX, maxY, maxZ), glm::vec2(0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1));
	vertices[6] = Vertex(glm::vec3(maxX, maxY, maxZ), glm::vec2(0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1));
	vertices[7] = Vertex(glm::vec3(maxX, minZ, maxZ), glm::vec2(1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1));
	vertices[8] = Vertex(glm::vec3(minX, minY, minZ), glm::vec2(0, 1), glm::vec3(0, -1, 0), glm::vec3(1, 1, 0));/* r&g wall */
	vertices[9] = Vertex(glm::vec3(minX, minY, maxZ), glm::vec2(1, 1), glm::vec3(0, -1, 0), glm::vec3(1, 1, 0));
	vertices[10] = Vertex(glm::vec3(maxX, minY, maxZ), glm::vec2(1, 0), glm::vec3(0, -1, 0), glm::vec3(1, 1, 0));
	vertices[11] = Vertex(glm::vec3(maxX, minY, minZ), glm::vec2(0, 0), glm::vec3(0, -1, 0), glm::vec3(1, 1, 0));
	vertices[12] = Vertex(glm::vec3(minX, maxY, minZ), glm::vec2(0, 1), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));/* green wall */
	vertices[13] = Vertex(glm::vec3(minX, maxY, maxZ), glm::vec2(1, 1), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	vertices[14] = Vertex(glm::vec3(maxX, maxY, maxZ), glm::vec2(1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	vertices[15] = Vertex(glm::vec3(maxX, maxY, minZ), glm::vec2(0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	vertices[16] = Vertex(glm::vec3(minX, minY, minZ), glm::vec2(1, 1), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)); /* red wall */
	vertices[17] = Vertex(glm::vec3(minX, minY, maxZ), glm::vec2(1, 0), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0));
	vertices[18] = Vertex(glm::vec3(minX, maxY, maxZ), glm::vec2(0, 0), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0));
	vertices[19] = Vertex(glm::vec3(minX, maxY, minZ), glm::vec2(0, 1), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0));
	vertices[20] = Vertex(glm::vec3(maxX, minY, minZ), glm::vec2(1, 1), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));/* r&b wall */
	vertices[21] = Vertex(glm::vec3(maxX, minY, maxZ), glm::vec2(1, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));
	vertices[22] = Vertex(glm::vec3(maxX, maxY, maxZ), glm::vec2(0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));
	vertices[23] = Vertex(glm::vec3(maxX, maxY, minZ), glm::vec2(0, 1), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));


}

unsigned int* BoundingBox::makeIndices() {
	unsigned int indices[] = { 0, 1, 2,
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
	return indices;
}
float checkMax(float maxAxies, float currAxies) {
	if (currAxies > maxAxies)
		return currAxies;
	return maxAxies;
}

float checkMin(float minAxies, float currAxies) {
	if (currAxies < minAxies)
		return currAxies;
	return minAxies;
}
glm::vec3 BoundingBox::findCenter(std::list<BoundingBox>& plist) {
	glm::vec3 ans;
	for (std::list<BoundingBox>::iterator i = plist.begin(); i != plist.end(); i++) {

		glm::vec3 vertex = i->position;
		glm::vec4 v4(vertex, 1);
		maxX = checkMax(maxX, vertex.x);
		maxY = checkMax(maxY, vertex.y);
		maxZ = checkMax(maxZ, vertex.z);
		minX = checkMin(minX, vertex.x);
		minY = checkMin(minY, vertex.y);
		minZ = checkMin(minZ, vertex.z);
	}
	ans = glm::vec3((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2);
	xyz = glm::vec3(minX, minY, minZ);
	Xyz = glm::vec3(maxX, minY, minZ);
	xYz = glm::vec3(minX, maxY, minZ);
	xyZ = glm::vec3(minX, minY, maxZ);
	XYz = glm::vec3(maxX, maxY, minZ);
	XyZ = glm::vec3(maxX, minY, maxZ);
	xYZ = glm::vec3(minX, maxY, maxZ);
	XYZ = glm::vec3(maxX, maxY, maxZ);

	return ans;
}

void BoundingBox::SVD() {

	Eigen::JacobiSVD<Eigen::MatrixXf> svd(S, Eigen::ComputeThinU | Eigen::ComputeThinV);
	U = svd.matrixU();
	uX = glm::normalize(glm::vec3(U(0, 0), U(1, 0), U(2, 0)));
	uY = glm::normalize(glm::vec3(U(0, 2), U(1, 2), U(2, 2)));
	uZ = glm::normalize(glm::vec3(U(0, 1), U(1, 1), U(2, 1)));
	rotmat = glm::mat4(glm::vec4(uX, 0), glm::vec4(uY, 0), glm::vec4(uZ, 0), glm::vec4(0, 0, 0, 1));
}

glm::vec3 Kdtree::findCenterOfMass(std::list<BoundingBox>& plist) {
	glm::vec3 ans(0);
	int n = 0;
	for (std::list<BoundingBox>::iterator i = plist.begin(); i != plist.end(); i++) {
		ans += i->position;
		n++;
	}
	ans *= 1 / n;
	return ans;
}

Eigen::MatrixXf Kdtree::findScatter(std::list<BoundingBox>& plist, glm::vec3 centerOfMass) {
	Eigen::MatrixXf Y(3, plist.size());
	int n = 0;
	for (std::list<BoundingBox>::iterator i = plist.begin(); i != plist.end(); i++) {
		Y(0, n) = (i->position.x - centerOfMass.x);
		Y(1, n) = (i->position.y - centerOfMass.y);
		Y(2, n) = (i->position.z - centerOfMass.z);
		n++;
	}
	return Y * Y.transpose();
}

void Kdtree::_makeTree(Node* head, std::list<BoundingBox>& plist, int depth)
{
	if (!plist.empty())
	{
		int k = N;
		int axis = depth % k;
		Eigen::MatrixXf scatter;
		glm::vec3 uX[3];
		if (bonus) {
			glm::vec3 centerOfMass = findCenterOfMass(plist);
			scatter = findScatter(plist, centerOfMass);
			Eigen::JacobiSVD<Eigen::MatrixXf> svd(scatter, Eigen::ComputeThinU | Eigen::ComputeThinV);
			Eigen::MatrixXf U = svd.matrixU();
			glm::vec3 uX[3] = { glm::vec3(U(0, 0), U(1, 0), U(2, 0)),
				glm::vec3(U(0, 2), U(1, 2), U(2, 2)),
				glm::vec3(U(0, 1), U(1, 1), U(2, 1)) };
		}

		std::list<BoundingBox> left_list;
		std::list<BoundingBox> right_list;

		BoundingBox median = Kdtree::findMedian(axis, plist, left_list, right_list, uX);
		median.setBoundingBox(plist, bonus, scatter);


		head->data = median;
		Node* left_node = new Node(k);
		Node* right_node = new Node(k);

		Kdtree::_makeTree(left_node, left_list, depth + 1);
		if (!left_list.empty()) head->left = left_node;

		Kdtree::_makeTree(right_node, right_list, depth + 1);
		if (!right_list.empty()) head->right = right_node;


	}
}