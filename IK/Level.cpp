#include <GL\glew.h>
#include "Level.h"
#include <iostream>

using namespace std;
using namespace glm;

Level::Level(glm::vec3 position, float angle, float hwRelation, float near, float far) : IK(position, angle, hwRelation, near, far)
{
	int ground = addTerrain("./res/textures/seafloor.jpg", 40, 0.2, 120);
	LevelShapeTransformation(ground, zGlobalTranslate, 0.85);

	for (int i = 0; i < 4; i++)
	{
		int leftWall = addTerrain("./res/textures/waterrock.jpg", 2, 1, 30);
		LevelShapeTransformation(leftWall, zGlobalTranslate, 0.45+2*i);
		LevelShapeTransformation(leftWall, xGlobalTranslate, 20);
		int rightWall = addTerrain("./res/textures/waterrock.jpg", 2, 1, 30);
		LevelShapeTransformation(rightWall, zGlobalTranslate, 0.45 + 2 * i);
		LevelShapeTransformation(rightWall, xGlobalTranslate, -20);
	}
	int bottomWall = addTerrain("./res/textures/waterrock.jpg", 42, 1, 2);
	LevelShapeTransformation(bottomWall, zGlobalTranslate, -8);


	int topWall = addTerrain("./res/textures/waterrock.jpg", 20, 1, 2);
	LevelShapeTransformation(topWall, xGlobalTranslate, 0.65);
	LevelShapeTransformation(topWall, zGlobalTranslate, 13);




	int item1 = addItem("./res/objs/testBoxNoUV.obj", "./res/textures/gold.jpg");
	LevelShapeTransformation(item1, xGlobalTranslate, -25);
	LevelShapeTransformation(item1, zGlobalTranslate, 17);
}

void Level::UpdateLevel()
{
	for(int i =0;i<LevelShapes.size();i++)
	{
		switch (LevelShapes.at(i)->getKind())
		{		
		case Shape::Item:					
			LevelShapeTransformation(i, xLocalRotate, 1.5);
			break;
		case Shape::Default:
			break;
		}
	}
	for(int i=0;i<linksNum;i++)
	{
		shapes[i]->snakeLinkPosition = glm::mat4(1);
		for (int j = 0; j <= i; j++)
		{
			shapes[i]->snakeLinkPosition = shapes[i]->snakeLinkPosition * shapes[j]->makeTrans();
		}
	
	}

}

bool Level::checkCollisionInLevel(int shape1, int shape2)
{
	return shapes.at(shape1)->isColliding(*LevelShapes.at(shape2));
}

bool Level::checkCollisionOfSnake(int shape)
{
	bool colliding = false;
	for (int i = 0;i < linksNum & !colliding;i++)
	{
		colliding = shapes.at(i)->isColliding(*LevelShapes.at(shape));
		if(colliding)
		{
			std::cout << "Link num " << i << " Colliding !!" << std::endl;
			switch(LevelShapes.at(shape)->getKind())
			{
			case Shape::Terrain:
				std::cout << "You Died" << std::endl;
				KillSnake();
				break;
			case Shape::Item:
				LevelShapes.erase(LevelShapes.begin() + shape);
				//addLink();
				std::cout << "Item Eaten" << std::endl;

				break;
			case Shape::Default:
				break;
			}
		}
		

	}
	return colliding;
}

bool Level::checkCollisionFullLevel()
{
	bool colliding = false;
	for (int i = 1; i < LevelShapes.size() & !colliding; i++)
	{
		colliding = checkCollisionOfSnake(i);
	}
	return colliding;

}

void Level::KillSnake()
{
	shapes.clear();
	gameOver = true;
	pickedShape = -1;
}


int Level::addTerrain(const std::string &textureFlieName, float x,float y, float z)
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(1, 1, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(1, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(1, 1, 1)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 1, 1)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 1, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 1, 1))
	};
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
	LevelShapes.push_back(new Shape(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), textureFlieName, Shape::Terrain));
	int terrain = LevelShapes.size() - 1;
	LevelShapeTransformation(terrain, xScale, x);
	LevelShapeTransformation(terrain, yScale, y);
	LevelShapeTransformation(terrain, zScale, z);
	return terrain;
}

int Level::addItem(const std::string& fileName, const std::string& textureFileName)
{
	LevelShapes.push_back(new Shape(fileName, textureFileName,Shape::Item));
	int item1 = LevelShapes.size() - 1;
	LevelShapeTransformation(item1, zScale, 1.1);
	LevelShapeTransformation(item1, yScale, 1.1);
	LevelShapeTransformation(item1, zScale, 1.1);
	LevelShapeTransformation(item1, yGlobalTranslate, 1.1);
	return item1;
}

void Level::createKDTreesForLevelShapes()
{
	for (int i = 0; i< LevelShapes.size(); i++)
	{
		LevelShapes.at(i)->makeKDTree(LevelShapes.at(i)->mesh->model);		
	}
}

void Level::levelDraw(int shaderIndx, int cameraIndx, bool drawAxis)
{
	glm::mat4 Normal = makeTrans();
	glm::mat4 MVP = cameras[0]->GetViewProjection() * Normal;

	shaders[shaderIndx]->Bind();

	for (int i = 0; i<LevelShapes.size(); i++)
	{
		mat4 Normal1 = mat4(1);
		
		mat4 MVP1 = MVP * Normal1;
		Normal1 = Normal * Normal1;

		MVP1 = MVP1 * LevelShapes[i]->makeTransScale(mat4(1));
		Normal1 = Normal1 * LevelShapes[i]->makeTrans();
		shaders[shaderIndx]->Update(MVP1, Normal1, i);
		LevelShapes.at(i)->draw(GL_TRIANGLES);
		
		//BB drawing
		/*
		Node *box = LevelShapes.at(i)->kdtree.getRoot();
		Shape *shape = new Shape(box->data.vertices, sizeof(box->data.vertices) / sizeof(box->data.vertices[0]),
			box->data.indices, sizeof(box->data.indices) / sizeof(box->data.indices[0]));
		glm::mat4 MVP2;
		glm::mat4 Normal2;
		if (LevelShapes.at(i)->kdtree.bonus) {
			MVP2 = MVP * LevelShapes.at(i)->translateMat[0] * glm::transpose(box->data.rotmat) * LevelShapes.at(i)->rotateMat * LevelShapes.at(i)->translateMat[1];
			Normal2 = Normal * LevelShapes.at(i)->translateMat[0] * glm::transpose(box->data.rotmat) *LevelShapes.at(i)->rotateMat * LevelShapes.at(i)->translateMat[1] * box->data.rotmat;
		}
		else {
			MVP2 = MVP * LevelShapes.at(i)->makeTrans(glm::mat4(1));
			Normal2 = Normal * LevelShapes.at(i)->makeTrans();
		}
		shaders[shaderIndx]->Update(MVP1, Normal1, linksNum);
		shape->draw(GL_LINE_LOOP);
		delete shape;
		*/
	}
}

void Level::LevelShapeTransformation(int shape,int type, float amt)
{
	Shape * s = LevelShapes[shape];

	switch (type)
	{
	case xLocalTranslate:
		s->myTranslate(glm::vec3(amt, 0.0, 0.0), 1);
		break;
	case yLocalTranslate:
		s->myTranslate(glm::vec3(0.0, amt, 0.0), 1);
		break;
	case zLocalTranslate:
		s->myTranslate(glm::vec3(0.0, 0.0, amt), 1);
		break;
	case xGlobalTranslate:
		s->myTranslate(glm::vec3(amt, 0.0, 0.0), 0);
		break;
	case yGlobalTranslate:
		s->myTranslate(glm::vec3(0.0, amt, 0.0), 0);
		break;
	case zGlobalTranslate:
		s->myTranslate(glm::vec3(0.0, 0.0, amt), 0);
		break;
	case xLocalRotate:
		s->myRotate(amt, glm::vec3(1, 0, 0), xAxis1);
		break;
	case yLocalRotate:
		s->myRotate(amt, glm::vec3(0, 1, 0), -1);
		break;
	case zLocalRotate:
		s->myRotate(amt, vec3(0, 0, 1), zAxis12);
		break;
	case xGlobalRotate:
		s->globalSystemRot(amt, glm::vec3(1, 0, 0), xAxis1);
		break;
	case yGlobalRotate:
		s->globalSystemRot(amt, glm::vec3(0, 1, 0), -1);
		break;
	case zGlobalRotate:
		s->globalSystemRot(amt, vec3(0, 0, 1), zAxis12);
		break;
	case xScale:
		s->myScale(vec3(amt, 1, 1));
		break;
	case yScale:
		s->myScale(vec3(1, amt, 1));
		break;
	case zScale:
		s->myScale(vec3(1, 1, amt));
		break;
	case xCameraTranslate: //camera plane translate

		break;
	case yCameraTranslate:

		break;
	case zCameraTranslate:

		break;
	default:
		break;
	}
}

Level::~Level()
{
}
