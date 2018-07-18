#pragma once
#include "IK.h"

class Level : public IK
{
public:

	std::vector<Shape*> LevelShapes;
	
	Level(glm::vec3 position, float angle, float hwRelation, float near, float far);
	virtual ~Level();
	void levelDraw(int shaderIndx, int cameraIndx, bool drawAxis);

	int addTerrain(const std::string &textureFlieName, float xScale, float yScale, float zScale);
	int addItem(const std::string& fileName, const std::string& textureFileName);


	void LevelShapeTransformation(int shape,int type, float amt);
	void UpdateLevel();
	bool checkCollisionInLevel(int shape1, int shape2);
	bool checkCollisionOfSnake(int shape);
	bool checkCollisionFullLevel();
	void createKDTreesForLevelShapes();

};


// add new shape holder for the level specific shapes and a new draw function for them

