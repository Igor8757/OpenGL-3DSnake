#include <..\GL\glew.h>
#include "Level.h"
#include <iostream>

using namespace std;
using namespace glm;
void Level::addItems() {
	

	addItem("./res/objs/coin.obj", "./res/textures/gold.jpg", Shape::ItemCoin, -25, 5);

	addItem("./res/objs/coin.obj", "./res/textures/gold.jpg", Shape::ItemCoin, -25, 12);

	addItem("./res/objs/diamond.obj", "./res/textures/diamond.jpg", Shape::ItemDiamond, 25, 65);

	addItem("./res/objs/ball.obj", "./res/textures/food.jpg", Shape::ItemFruit, 27, 10);

	addItem("./res/objs/coin.obj", "./res/textures/gold.jpg", Shape::ItemCoin, -22, 50);
	addItem("./res/objs/coin.obj", "./res/textures/gold.jpg", Shape::ItemCoin, -7, 50);


	int enemy1 = addEnemy1(0, enemyHeight, 17);
	AddMovement(enemy1, 8, 0.03, zGlobalTranslate);

	int enemy2 = addEnemy1(12, enemyHeight, 4);
	AddMovement(enemy2, 5, 0.03, xGlobalTranslate);

	int enemy3 = addEnemy1(12, enemyHeight, 30);
	AddMovement(enemy3, 4, 0.025, zGlobalTranslate);

	int enemy4 = addEnemy1(-12, enemyHeight, 40);
	AddMovement(enemy4, 4, 0.025, xGlobalTranslate);

}
Level::Level(glm::vec3 position, float angle, float hwRelation, float near, float far) : IK(position, angle, hwRelation, near, far)
{
	wallHeight = 6;
	enemyHeight = 0.3;
	int ground = addTerrain("./res/textures/seafloor.jpg", 40, 0.2, 120,0 , 0.85);
	LevelShapeTransformation(ground, yGlobalTranslate, -5);


	for (int i = 0; i < 5; i++)
	{
		 addTerrain("./res/textures/waterrock.jpg", 2, wallHeight, 23, 20 , 0.55 + 2 * i);
		 addTerrain("./res/textures/waterrock.jpg", 2, wallHeight, 23, -20, 0.55 + 2 * i);

	}
	addTerrain("./res/textures/waterrock.jpg", 42, wallHeight, 3, 0, -4);

	addTerrain("./res/textures/waterrock.jpg", 25, wallHeight, 2, 0.65, 13);

	 addTerrain("./res/textures/waterrock.jpg", 2, wallHeight, 10, 2, 5);

	addTerrain("./res/textures/waterrock.jpg", 30, wallHeight, 2, 0.4, 40);

	addTerrain("./res/textures/waterrock.jpg", 23, wallHeight, 2, -0.75, 20);

	addTerrain("./res/textures/waterrock.jpg", 23, wallHeight, 2, -0.75 , 50);

	addTerrain("./res/textures/waterrock.jpg", 23, wallHeight, 2, 0, 60);

	addTerrain("./res/textures/waterrock.jpg", 15, wallHeight, 2, -1.5, 67);

	addTerrain("./res/textures/waterrock.jpg", 15, wallHeight, 2, 1.5, 67);

	addTerrain("./res/textures/waterrock.jpg", 2, wallHeight, 12, 4.8, 12.25);

	addTerrain("./res/textures/waterrock.jpg", 2, wallHeight, 12, -4.8, 12.25);
	addItems();
	
}

int Level::addEnemy1(float x, float y, float z)
{
	int enemy = addEnemy("./res/objs/monkey3.obj", "./res/textures/gold.jpg");
	LevelShapeTransformation(enemy, xScale, 2);
	LevelShapeTransformation(enemy, yScale, 2);
	LevelShapeTransformation(enemy, zScale, 2);
	LevelShapeTransformation(enemy, xGlobalTranslate, x);
	LevelShapeTransformation(enemy, yGlobalTranslate, y);
	LevelShapeTransformation(enemy, zGlobalTranslate, z);
	LevelShapeTransformation(enemy, zGlobalRotate, 180);
	LevelShapeTransformation(enemy, yGlobalRotate, 90);
	return enemy;
}

void Level::UpdateLevel()
{
	for(int i =0;i<LevelShapes.size();i++)
	{
		MoveShape(i);
		switch (LevelShapes.at(i)->getKind())
		{		
		case Shape::ItemFruit:					
			LevelShapeTransformation(i, yLocalRotate, 1.5);
			break;
		case Shape::ItemCoin:
			LevelShapeTransformation(i, yLocalRotate, 1.5);
			break;
		case Shape::ItemDiamond:
			LevelShapeTransformation(i, yLocalRotate, 1.5);
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
void Level::levelStartOver() {
	std::vector<Shape*>::iterator it;
	it = LevelShapes.begin();
	for (int i = 0; i< LevelShapes.size(); i++) {
		if (LevelShapes.at(i)->getKind() == Shape::Terrain) {
			it++;
			continue;
		}
		LevelShapes.erase(it);
		it++;
	}
	StartOver();
	addItems();
}
bool Level::checkCollisionOfSnake(int shape)
{
	
	bool colliding = false;
	for (int i = 0;i < linksNum & !colliding;i++)
	{
		colliding = shapes.at(i)->isColliding(*LevelShapes.at(shape));
		if(colliding)
		{
			clock_t this_time = clock();
			std::cout << "Link num " << i << " Colliding with shape num " << shape <<" !!" << std::endl;
			switch(LevelShapes.at(shape)->getKind())
			{
			case Shape::Terrain:
				std::cout << "You hit wall and died" << std::endl;
				KillSnake();
				break;
			case Shape::Enemy:
				std::cout << "You were killed" << std::endl;
				KillSnake();
				break;
			case Shape::ItemFruit:
				LevelShapes.erase(LevelShapes.begin() + shape);
				addRemoveLinks(this_time, true);
				std::cout << "Fruit Eaten" << std::endl;
				Points += 100;
				std::cout << "Points : " << Points << std::endl;

				break;
			case Shape::ItemCoin:
				LevelShapes.erase(LevelShapes.begin() + shape);
				std::cout << "Box Eaten" << std::endl;
				Points += 200;
				std::cout << "Points : " << Points << std::endl;

				break;
			case Shape::ItemDiamond:
				LevelShapes.erase(LevelShapes.begin() + shape);
				std::cout << "Diamond Eaten" << std::endl;
				Points += 500;
				std::cout << "Points : " << Points << std::endl;

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
	checkSnakeBulletCollision();
	if (checkCollisionOfSnakeHead())
		return false;
	bool colliding = false;
	for (int i = 1; i < LevelShapes.size() & !colliding; i++)
	{
		colliding = checkCollisionOfSnake(i);
	}
	return colliding;

}

void Level::checkSnakeBulletCollision()
{
	if (snakeShots.size() <= 0) {
		return;
	}
	for(int i = 0;i<snakeShots.size();i++)
	{
		for(int j = 1;j < LevelShapes.size();j++)
		{
			if (snakeShots.at(i)->shot->isColliding(*LevelShapes.at(j)))
			{
				switch (LevelShapes.at(j)->getKind())
				{
				case Shape::Terrain:
					std::cout << "Bullet hit wall." << std::endl;
					break;
				case Shape::Enemy:
					std::cout << "Bullet hit enemy." << std::endl;
					LevelShapes.erase(LevelShapes.begin() + j);
					break;
				case Shape::ItemFruit:
					break;
				case Shape::Default:
					break;
				}
				snakeShots.erase(snakeShots.begin() + i);
				return;
			}
		}
	}
}

bool Level::checkCollisionOfSnakeHead()
{
	for (int i = 3; i < linksNum; i++)
	{		
		if (shapes.at(0)->isColliding(*shapes.at(i)))
		{
			std::cout << "You ate yourself and died, good job" << std::endl;

			KillSnake();
			return true;
		}
	}
	return false;
}

void Level::KillSnake()
{
	shapes.clear();
	gameOver = true;
	pickedShape = -1;
}

void Level::AddMovement(int shape, float pathLength, float pace, int type)
{
	LevelShapes.at(shape)->movementEnabled = true;
	LevelShapes.at(shape)->movementPath = pathLength;
	LevelShapes.at(shape)->movementPace = pace;
	LevelShapes.at(shape)->movementType = type;
}

void Level::MoveShape(int shape)
{
	if (!LevelShapes.at(shape)->movementEnabled)
		return;

	if (LevelShapes.at(shape)->negDirect && LevelShapes.at(shape)->currentPos <= -LevelShapes.at(shape)->movementPath)
	{
		LevelShapes.at(shape)->negDirect = false;
	}	
	else if(LevelShapes.at(shape)->negDirect)
	{
		LevelShapeTransformation(shape, LevelShapes.at(shape)->movementType, -LevelShapes.at(shape)->movementPace);
		LevelShapes.at(shape)->currentPos -= LevelShapes.at(shape)->movementPace;
	}
	else if(LevelShapes.at(shape)->currentPos < LevelShapes.at(shape)->movementPath){
		LevelShapeTransformation(shape, LevelShapes.at(shape)->movementType, LevelShapes.at(shape)->movementPace);
		LevelShapes.at(shape)->currentPos += LevelShapes.at(shape)->movementPace;
	}
	else if (LevelShapes.at(shape)->currentPos >= LevelShapes.at(shape)->movementPath) {
		LevelShapes.at(shape)->negDirect = true;
	}
	
}


int Level::addTerrain(const std::string &textureFlieName, float x,float y, float z, float posx, float posz)
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
	LevelShapeTransformation(terrain, zGlobalTranslate, posz);
	LevelShapeTransformation(terrain, xGlobalTranslate, posx);
	return terrain;
}

int Level::addItem(const std::string& fileName, const std::string& textureFileName, Shape::objKind kind, float x, float z)
{
	LevelShapes.push_back(new Shape(fileName, textureFileName,kind));
	int item1 = LevelShapes.size() - 1;
	LevelShapeTransformation(item1, xScale, 1.1);
	LevelShapeTransformation(item1, yScale, 1.1);
	LevelShapeTransformation(item1, zScale, 1.1);
	LevelShapeTransformation(item1, yGlobalTranslate, 1.3);
	LevelShapeTransformation(item1, xGlobalTranslate, x);
	LevelShapeTransformation(item1, zGlobalTranslate, z);
	return item1;
}

int Level::addEnemy(const std::string& fileName, const std::string& textureFileName)
{
	LevelShapes.push_back(new Shape(fileName, textureFileName, Shape::Enemy));
	int enemy = LevelShapes.size() - 1;
	return enemy;
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
