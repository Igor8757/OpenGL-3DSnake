#pragma once
#include "IK.h"


class Level : public IK
{
private:
	int wallHeight = 0;
	int enemyHeight =0;
	std::string message = "";
	bool winning = false;
public:
	inline bool getWinning() {
		return winning;
	}
	inline void setWinning() {
		 winning =false;
	}
	inline std::string getMessage() {
		return message;
	}
	inline void setMessage(std::string other) {
		this->message = other;
	}
	std::vector<Shape*> LevelShapes;
	int Points = 0;

	Level(glm::vec3 position, float angle, float hwRelation, float near, float far);



	//Shape Functions
	int addTerrain(const std::string &textureFlieName, float x, float y, float z, float posx, float posz);
	int addItem(const std::string& fileName, const std::string& textureFileName, Shape::objKind kind, float x, float z);
	int addEnemy(const std::string& fileName, const std::string& textureFileName);
	void LevelShapeTransformation(int shape, int type, float amt);
	void levelStartOver();
	int addEnemy1(float x, float y, float z);
	int addWinShape();

	void addItemsEasy();
	void addItemsHard();

	//Level Function
	void UpdateLevel();
	void KillSnake();
	void AddMovement(int shape, float pathLength, float pace, int type);
	void MoveShape(int shape);
	void InitLevel();

	//Collision Functions
	void createKDTreesForLevelShapes();
	bool checkCollisionInLevel(int shape1, int shape2);
	bool checkCollisionOfSnake(int shape);
	bool checkCollisionFullLevel();
	void checkSnakeBulletCollision();
	bool checkCollisionOfSnakeHead();

	// Other Functions
	void levelDraw(int shaderIndx, int cameraIndx, bool drawAxis, int camType);
	virtual ~Level();

};



