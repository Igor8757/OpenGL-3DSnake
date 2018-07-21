#pragma once   //maybe should be static class
#include "GLFW\glfw3.h"
#include "IK.h"
#include "../IK/Level.h"


const int DISPLAY_WIDTH = 1200;
const int DISPLAY_HEIGHT = 800;
const float FAR = 100.0f;
const float NEAR = 1.0f;
const float CAM_ANGLE = 60.0f;
float relation = (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT;


Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");

Level scn(glm::vec3(0.0f, 5.0f, -40.0f), CAM_ANGLE, relation, NEAR, FAR);

float factor = 1.0;

double x1 = 0, x2 = 0;
double ys1 = 0, y2 = 0;
float depth;








void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		clock_t this_time = clock();
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_RIGHT:
			if (scn.cameraMode)
				scn.shapeTransformation(scn.zGlobalRotate, -5.1f);
			else if (scn.boxMode) {
				scn.shapeTransformation(scn.xGlobalTranslate, -0.5f);
			}
			else
			{
				scn.shapeTransformation(scn.zLocalRotate, 5.f);
			}		//						cout<< "right: "<<endl;

			break;
		case GLFW_KEY_LEFT:
			if (scn.cameraMode) {
				scn.shapeTransformation(scn.zGlobalRotate, 5.1f);
			}
			else if (scn.boxMode) {

				scn.shapeTransformation(scn.xGlobalTranslate, 0.5f);
			}
			else {
				scn.shapeTransformation(scn.zLocalRotate, -5.f);
			}
			//					cout<< "left: "<<endl;
			break;
		case GLFW_KEY_UP:
			//cout<< "up: "<<endl;
			if (scn.cameraMode)
			{
				scn.shapeTransformation(scn.xGlobalRotate, 5.1f);
			}
			else if (scn.boxMode) {
				scn.shapeTransformation(scn.zGlobalTranslate, 0.5f);
			}
			else {
				scn.shapeTransformation(scn.xGlobalRotate, 5.f);
			}
			break;
		case GLFW_KEY_DOWN:
			if (scn.cameraMode)
			{
				scn.shapeTransformation(scn.xGlobalRotate, -5.f);
			}
			else if (scn.boxMode) {

				scn.shapeTransformation(scn.zGlobalTranslate, -0.5f);
			}
			else {
				scn.shapeTransformation(scn.xGlobalRotate, -5.f);
				//cout<< "down: "<<endl;
			}
			break;
		case GLFW_KEY_SPACE:
			if (scn.isActive()) {
				std::cout << "::Space:: IK deactivated " << std::endl;
				scn.deactivateIK();
			}
			else {
				std::cout << "::Space:: IK activated " << std::endl;
				scn.activateIK();
				scn.makeChange();
				//scn.printData();
			}

			break;
		case GLFW_KEY_N:
			scn.nextLink();
			printf("::N:: Next Link, the current link is : %d\n", scn.getPickedShape());
			break;
		case GLFW_KEY_P:
			scn.Pause();
			break;
		case GLFW_KEY_B:
			scn.selectBox();
			break;
		case GLFW_KEY_C:
			scn.selectCamera();
			break;
		case GLFW_KEY_W:
			if(!scn.paused)
			{
				//clock_t this_time = clock();
				scn.addRotTime(this_time);
				//scn.addRotTime(scn.getDestination(0));
				scn.addVectorToShapes(glm::vec2(scn.xLocalRotate, 3.f));
				//scn.shapeTransformation(scn.xLocalRotate, 5.f);
			}		

			break;
		case GLFW_KEY_S:
			if (!scn.paused)
			{
				//clock_t this_time = clock();
				scn.addRotTime(this_time);
				//scn.addRotTime(scn.getDestination(0));
				scn.addVectorToShapes(glm::vec2(scn.xLocalRotate, -3.f));
				//scn.shapeTransformation(scn.xLocalRotate, -5.f);
			}
			break;
		case GLFW_KEY_D:
			if (!scn.paused)
			{
				//clock_t this_time = clock();
				scn.addRotTime(this_time);
				//scn.addRotTime(scn.getDestination(0));
				//scn.shapeTransformation(scn.zGlobalTranslate, -0.5f);
				scn.addVectorToShapes(glm::vec2(scn.yLocalRotate, -5.f));
				//scn.shapeTransformation(scn.yLocalRotate, -5.f);
			}
			break;
		case GLFW_KEY_A:
			if (!scn.paused)
			{
				//clock_t this_time = clock();
				scn.addRotTime(this_time);
				//scn.addRotTime(scn.getDestination(0));
				scn.addVectorToShapes(glm::vec2(scn.yLocalRotate, 5.f));
				//scn.shapeTransformation(scn.yLocalRotate, 5.f);
			}
			break;
		case GLFW_KEY_KP_8:
			scn.getCamera()->MoveUp(5);
			break;
		case GLFW_KEY_KP_2:
			scn.getCamera()->MoveUp(-5);
			break;
		case GLFW_KEY_KP_4:
			scn.getCamera()->MoveRight(5);
			break;
		case GLFW_KEY_KP_6:
			scn.getCamera()->MoveRight(-5);
			break;
		case GLFW_KEY_KP_7:
			scn.getCamera()->MoveForward(-5);
			break;
		case GLFW_KEY_KP_9:
			scn.getCamera()->MoveForward(5);
			break;
		
		case GLFW_KEY_Z:
			scn.addRemoveLinks(this_time,true);
			//printf("::N:: Next Link, the current link is : %d\n", scn.getPickedShape());
			break;
		case GLFW_KEY_X:
			//scn.addRemoveLinks(this_time, false);
			if (scn.GetLinkNum() <= 1) {
				break;
			}
			scn.removeLink();
			scn.shot(this_time);
			//printf("::N:: Next Link, the current link is : %d\n", scn.getPickedShape());
			break;
		case GLFW_KEY_V:
			//scn.addRemoveLinks(this_time,false);
			scn.addLink();
			//printf("::N:: Next Link, the current link is : %d\n", scn.getPickedShape());
			break;
		default:
			break;
		}
	}
}
void updatePosition(double xpos, double ypos)
{
	x1 = x2;
	x2 = xpos;
	ys1 = y2;
	y2 = ypos;
}



void window_size_callback(GLFWwindow* window, int width, int height)
{

	scn.resize(width, height, NEAR, FAR);
	relation = (float)width / (float)height;
}