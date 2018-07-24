#include "IK.h"
#include <iostream>

using namespace std;
using namespace glm;


	void printArr(vector<int> arr)
	{
		
		std::cout<<"array"<<std::endl;
		for (int i = 0; i < arr.size(); i++)
		{
			std::cout<<i<<"  ";
		}
		std::cout<<std::endl;
		for(int p : arr)
		{
			
			std::cout<<p<<"  ";
		}
		cout<<std::endl;
	}
	
	IK::IK(void)
	{
		boxMode = false;
		cameraMode = false;
		isIKactive = false;
		delta = 1e-1;
		destPosition = vec3(1,0,0);
		tipPosition = vec3(0,0,2*linksNum*scaleFactor);
		maxDistance = linksNum*2.0f*scaleFactor;
	}

	IK::IK(vec3 position,float angle,float hwRelation,float near, float far) : Scene(position,angle,hwRelation,near,far)
	{
		cameraMode = false;
		isIKactive = false;
		boxMode = false;
		delta = 0.5;
		destPosition = vec3(1,0,0);
		tipPosition = vec3(0,0,2*linksNum*scaleFactor);
		maxDistance = linksNum*scaleFactor;
		//maxDistance = linksNum * 2.0f*scaleFactor;
		
	}

	IK::~IK(void)
	{
	}
	void IK::StartOver() {
		cameras.clear(); 
		rotPositions.clear();
		TimeOfRotiains.clear();
		shaders.clear();
		shapes.clear();
		addRemoveLinksVec.clear();
		tranlateBack.clear();
		chainParents.clear();
		snakeShots.clear();
		objectsShots.clear();
		newPositions.clear();
		jointPositions.clear();
		jointDistances.clear();
		newPositions.clear();
		euler.clear();
		//glLineWidth(3);
		const float FAR = 100.0f;
		const float NEAR = 1.0f;
		const float CAM_ANGLE = 60.0f;
		float relation = (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT;
		cameras.push_back(new Camera(glm::vec3(0.0f, 5.0f, -48.0f), CAM_ANGLE, relation, NEAR, FAR));
		cameras.push_back(new Camera(glm::vec3(0.0f, 0.0f,  3.0f), CAM_ANGLE, relation, NEAR, FAR));
		cameras.at(1)->Pitch(-50.0f);
		//	axisMesh = new Shape(axisVertices,sizeof(axisVertices)/sizeof(axisVertices[0]),axisIndices, sizeof(axisIndices)/sizeof(axisIndices[0]));
		pickedShape = -1;
	}
	void IK::init(Vertex *vertices, unsigned int *indices, int verticesSize, int indicesSize, bool cmaeraMode)
	{
		toopView = cmaeraMode;
		for (int i = 0; i < linksNum + 1; i++) {
			newPositions.push_back(glm::vec3(0, 0, 0));			
		}
		myRotate(-90.0f,vec3(1,0,0),-1);
		//addShape(vertices, verticesSize, indices, indicesSize,"./res/textures/plane.png",-1);
		addShape(0,2,"./res/textures/plane.png",-1,snake);
		pickedShape = 0;
		shapes.at(pickedShape)->linkNumber = pickedShape;
		shapeTransformation(zScale,scaleFactor);
		shapesNormal.push_back(glm::mat4(1));
		for (int i = 1; i < linksNum-1; i++)
		{
			pickedShape = i;
			addShape(1,1,"./res/textures/plane.png",-1,snake);
			shapes.at(pickedShape)->linkNumber = pickedShape;

			shapeTransformation(zScale,scaleFactor);	
			shapesNormal.push_back (glm::mat4(1));
			shapeTransformation(zGlobalTranslate,1.0);
			setParent(i,i-1);
		}
		shapesNormal.push_back(glm::mat4(1));
		pickedShape = linksNum-1;
		addShape(0,3,"./res/textures/plane.png",-1,snake);
		shapes.at(pickedShape)->linkNumber = pickedShape;
		shapeTransformation(zScale,scaleFactor);	
			
		shapeTransformation(zGlobalTranslate,1.0);
		setParent(linksNum-1,linksNum-2);

		chainParents.push_back(-1);
		pickedShape = 0;

		for (int i = 0; i < linksNum; i++) {
			euler.push_back(EulerAngles());
			shapes.at(i)->isSnake = true;
		}
	
		//glm::vec3 camera =  getTipPosition(0);
		//cameras.push_back(new Camera(camera, 0, 12/8, 1, 100));
		
		pickedShape = 0;
		shapeTransformation(yLocalRotate, -180.f);
		//updateData();
	}



	void IK::addShape(int CylParts,int linkPosition,int parent)
	{
		
		__super::addShape(CylParts,linkPosition,parent);
	}

	void IK::addShape(int CylParts,int linkPosition,const std::string& fileName,int parent,int kind)
	{	
		__super::addShape(CylParts,linkPosition,fileName,parent, kind);
	}

	void IK::addShape(int type, int parent) 
	{
		
		__super::addShape(type,parent);
	}

	void IK::addShape(const std::string& fileName, int parent)
	{
		
		__super::addShape(fileName,parent);
	}

	void IK::addShape(const std::string& fileName,const std::string& textureFileName, int parent,int kind)
	{
		
		__super::addShape(fileName,textureFileName,parent,kind);
	}

	void IK::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent)
	{
		
		__super::addShape(vertices,numVertices,indices,numIndices,parent);
	}
	
	void IK::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName, int parent,int kind)
	{
		
		__super::addShape(vertices,numVertices,indices,numIndices,textureFlieName,parent,kind);
	}
	
	void IK::calculateStep(bool EulerVersion)
	{
	

	}

	void IK::makeChange()
	{
		// FABRIK algorithm
		updateData();

		
		if (!reachable) {
			float signX = 1;
			float signZ = 1;
			if (getDestination(linksNum).x - getDestination(0).x > 0)
				signX = -1;
			//if (getDestination(linksNum).z - getDestination(i).z < 0)
				//signZ = -1;
			float r, gamma;;
			glm::vec3 Zaxis = glm::vec3(0, 0, 1);//shapes[0]->getVectorInSystem(glm::mat4(1), glm::vec3(0, 0, 1));
			glm::vec3 Xaxis = glm::vec3(1, 0, 0); //shapes[0]->getVectorInSystem(glm::mat4(1), glm::vec3(1, 0, 0));
			r = glm::distance(getDestination(0), destPosition);
			gamma = distanceBetweenJoints / r;
			glm::vec3 Vp = (1 - gamma) * destPosition + newPositions.at(0)* gamma;
			
			glm::vec3 V = glm::vec3(glm::cross(Zaxis, Vp));
			V = normalize(V);
			Zaxis = normalize(Zaxis);
			Xaxis = normalize(Xaxis);
			Vp = normalize(Vp);
			float theta = glm::acos(glm::clamp(glm::dot(Zaxis, Vp), -0.99f, 0.99f));
			float phi = -glm::acos(glm::clamp(glm::dot(Xaxis, V), -0.99f, 0.99f));;
			for (int i = 1; i < linksNum ; i++) {
				shapes[i]->phi = glm::mat4(1); 
				shapes[i]->theta = glm::mat4(1);
				shapes[i]->psi = glm::mat4(1) ;
			}
			shapes[0]->buildAngMatrix(zAxis1, glm::cos(phi), signX*glm::sin(phi));
			shapes[0]->buildAngMatrix(xAxis1,  glm::cos(theta),  glm::sin(theta));

		}
		else {
			glm::vec3 b = getTipPosition(0);


			// FORWARD :
			newPositions.at(linksNum) = destPosition;
			newPositions.at(linksNum - 1) = destPosition;
			float r, gamma;
			for (int i = linksNum - 1; i >= 0; i--) {
				r = glm::distance(newPositions[i + 1], getDestination(i));
				gamma = distanceBetweenJoints / r;
				newPositions[i] = (1 - gamma) * newPositions[i + 1] + gamma * getDestination(i);

			}
			// BACKWARD :
			newPositions.at(0) = glm::vec3(0, 0, 0);
			for (int i = 0; i < linksNum - 1; i++) {
				r = glm::distance(newPositions.at(i + 1), newPositions.at(i));
				gamma = distanceBetweenJoints / r;
				newPositions.at(i + 1) = (1 - gamma) * newPositions.at(i) + gamma * newPositions.at(i + 1);
			}
			IKNewPosTrans();

		}

	}

	void IK::IKNewPosTrans() {
		// Maybe change this  (add the current root position and not (0,0,0)) (getTipPosition(1)-(0,0,0))
		glm::vec3 Zaxis;
		glm::vec3 Xaxis;
		glm::vec3 Vp;
		glm::vec3 V;

		glm::vec3 a, b, newZ;
		float theta, phi, cosPhi, cosTheta, sinPhi, sinTheta, signX, signZ;


		for (int i = 0; i < linksNum; i++) {
			float signX = 1;
			float signZ = 1;
			if (getDestination(linksNum).x - getDestination(0).x > 0)
				signX = -1;
			//if (getDestination(linksNum).z - getDestination(i).z < 0)
			//signZ = -1;
			float r, gamma;
			glm::vec3 Zaxis;
			glm::vec3 Xaxis;
			
				//float signX = 1;
				mat4 axiesMat = mat4(1);
				for (int j = 0; j < i ; j++)//get the new axies - one of this two sepused to work
				{
					axiesMat = axiesMat*shapes[j]->makeTrans() ;
					//axiesMat = shapes[j]->makeTrans()*axiesMat ;
				}

				
				Xaxis = glm::vec3(axiesMat[0]);
				Zaxis = glm::vec3(axiesMat[2]);

				
			glm::vec3 Vp = newPositions.at(i + 1) - newPositions.at(i);
			Zaxis = normalize(Zaxis);
			Xaxis = normalize(Xaxis);
			if (Vp != glm::vec3(0, 0, 0))
				Vp = normalize(Vp);
			glm::vec3 V = glm::vec3(glm::cross(Zaxis, Vp));
			if (V != glm::vec3(0, 0, 0))
				V = normalize(V);
			//V = normalize(V);
		
			float theta = glm::acos(glm::clamp(glm::dot(Zaxis, Vp), -0.99f, 0.99f));
			float phi = -glm::acos(glm::clamp(glm::dot(Xaxis, V), -0.99f, 0.99f));;
			
			shapes[i]->buildAngMatrix(zAxis1, glm::cos(phi), signX*glm::sin(phi));
			shapes[i]->buildAngMatrix(xAxis1, glm::cos(theta), glm::sin(theta));
		}


		/*
		for (int i = 0; i < linksNum; i++) {
		/*if (i == 0) {
		Zaxis = shapes[i]->getVectorInSystem(glm::mat4(1), glm::vec3(0, 0, 1));
		Xaxis = shapes[i]->getVectorInSystem(glm::mat4(1), glm::vec3(1, 0, 0));
		}
		else {
		Zaxis = shapes[i + 1]->getVectorInSystem(shapes[i]->makeTrans(glm::mat4(1)), glm::vec3(0, 0, 1));
		Xaxis = shapes[i + 1]->getVectorInSystem(shapes[i]->makeTrans(glm::mat4(1)), glm::vec3(1, 0, 0));
		}
		Zaxis = shapes[i]->getVectorInSystem(glm::mat4(1), glm::vec3(0, 0, 1));
		Xaxis = shapes[i]->getVectorInSystem(glm::mat4(1), glm::vec3(1, 0, 0));
		signX = 1;
		signZ = 1;
		if (getDestination(linksNum).x - getDestination(i).x < 0)
		signX = -1;
		if (getDestination(linksNum).z - getDestination(i).z < 0)
		signZ = 1;
		newZ = newPositions[i + 1];
		b = glm::dot(Zaxis, newZ)*Zaxis;
		a = newZ - b;
		cosTheta = glm::clamp(glm::dot(glm::normalize(a), glm::vec3(0, 0, 1)), -0.99f, 0.99f);
		cosPhi = glm::clamp(glm::dot(glm::normalize(newZ), glm::vec3(1, 0, 0)), -0.99f, 0.99f);
		sinPhi = glm::sqrt(1 - cosPhi * cosPhi);
		sinTheta = glm::sqrt(1 - cosTheta * cosTheta);

		//IKrotate(i, theta, phi);
		shapes[i]->buildAngMatrix(xAxis1, signX*cosTheta, signX*sinTheta);
		shapes[i]->buildAngMatrix(zAxis1, signZ*cosPhi, signZ*sinPhi);
		}
		*/
	}
	void IK::selectBox() {
		if (pickedShape == getShapesSize() - 1) {
			printf("::B:: First Link, the current link is : %d\n", pickedShape);
			pickedShape = 0;
			boxMode = false;
			cameraMode = false;
		}
		else {
			printf("::B:: Box, the current link is : %d\n", pickedShape);
			boxMode = true;
			cameraMode = false;
			pickedShape = getShapesSize() - 1;
		}
	}

	void IK::selectCamera() {
		/*if (!cameraMode) {
			printf("::C:: Camera Mode On\n");
			cameraMode = true;
			boxMode = false;
		}
		else {
			printf("::C:: Camera Mode On\n");
			boxMode = false;
			cameraMode = false;
		}*/
		toopView = !toopView;
	}

	void IK::getDistances() {
		jointDistances.clear();
		for (int i = 0; i < getShapesSize() - 2; i++) {
			float dist = glm::distance(getTipPosition(i+1), getTipPosition(i));
			jointDistances.push_back(dist);
		}
	}

	void IK::updateData()
	{
		destPosition = getDestination(linksNum);
		tipPosition = getTipPosition(linksNum - 1);
		getDistances();
		distanceBetweenJoints = jointDistances.at(0);
		distanceRootToTagret = glm::distance(glm::vec3(0, 0, 0), destPosition);
		reachable = distanceRootToTagret <= maxDistance;
	}

	void IK::printData() {
		std::cout << "Number of joints : " << (getShapesSize() - 1) << endl;
		std::cout << "Distance between joints : " << distanceBetweenJoints << endl;
		std::cout << "Distance root - target : " << distanceRootToTagret << endl;
		std::cout << "Max Distance : " << maxDistance << endl;
		std::cout << "Destination Position  : (" << destPosition.x << " " << destPosition.y << " " << destPosition.z << ")" << endl;
		std::cout << "Tip Position  : (" << tipPosition.x << " " << tipPosition.y << " " << tipPosition.z << ")" << endl;
		std::cout << "Reachable ? : " << reachable << endl;

	}

	void IK::setLinkNum(int numOflinks) {
		linksNum = numOflinks;
		Links = linksNum;
	}




	