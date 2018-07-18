#include <iostream>

#include "display.h"
#include "inputManager.h"
#include "shader.h"


static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;


int main(int argc, char** argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
	};

	unsigned int indices[] = {0, 1, 2,
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
	Scene scn(glm::vec3(0.0f, 0.0f, -15.0f), 60.0f, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 100.0f);
	
	scn.addShape(vertices, sizeof(vertices)/sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]),"./res/plane.png");
	for(int i=0;i<squarsNum*squarsNum*squarsNum;i++)
	{
		scn.addShape(0);
	}
	//for(int i=0;i<squarsNum*squarsNum*squarsNum;i++)
	//{
	//	scn.addShape(vertices, sizeof(vertices)/sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]),"./res/plane.png");
	//}
	for(int i =-squarsNum + squarsNum/2 + squarsNum%2 ; i <= squarsNum - squarsNum/2 - 1;i++)
	{
		for(int j= -squarsNum + squarsNum/2 + squarsNum%2; j <=squarsNum - squarsNum/2 -1 ; j++)
		{
			for(int k = -squarsNum + squarsNum/2 + squarsNum%2;k<=squarsNum - squarsNum/2 -1;k++)
			{
				scn.setPicked((i + squarsNum/2)*squarsNum*squarsNum + (j + squarsNum/2)*squarsNum + k + squarsNum/2);
				if(j!=0)
					scn.shapeTransformation(1,j*2);
				if(i != 0)
					scn.shapeTransformation(0,i*2);
				if(k != 0)
					scn.shapeTransformation(2,k*2);
				if(squarsNum % 2==0)
				{
					scn.shapeTransformation(0,1);
					scn.shapeTransformation(1,1);
					scn.shapeTransformation(2,1);
				}
			}
		}
	}
	//scn.addShape("./res/monkey3.obj","./res/grass.bmp");
	//Shader shader("./res/basicShader");
	scn.addShader("./res/basicShader");
	scn.addShader("./res/pickingShader");
	//inputManager input = inputManager();
	while(inputManager::play(scn))
	{

		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);

		scn.draw(0,0);

		display.SwapBuffers();

	}

	return 0;
}
