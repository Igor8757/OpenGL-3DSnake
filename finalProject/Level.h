#pragma once
#include "IK.h"


class Level : public IK
{
public:

	std::vector<Shape*> LevelShapes;
	int Points = 0;

	Level(glm::vec3 position, float angle, float hwRelation, float near, float far);



	//Shape Functions
	int addTerrain(const std::string &textureFlieName, float xScale, float yScale, float zScale);
	int addItem(const std::string& fileName, const std::string& textureFileName);
	int addEnemy(const std::string& fileName, const std::string& textureFileName);
	void LevelShapeTransformation(int shape, int type, float amt);

	int addEnemy1(float x, float y, float z);

	//Level Function
	void UpdateLevel();
	void KillSnake();
	void AddMovement(int shape, float pathLength, float pace, int type);
	void MoveShape(int shape);

	//Collision Functions
	void createKDTreesForLevelShapes();
	bool checkCollisionInLevel(int shape1, int shape2);
	bool checkCollisionOfSnake(int shape);
	bool checkCollisionFullLevel();
	void checkSnakeBulletCollision();
	bool checkCollisionOfSnakeHead();

	// Other Functions
	void levelDraw(int shaderIndx, int cameraIndx, bool drawAxis);
	virtual ~Level();

};



