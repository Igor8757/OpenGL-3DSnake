#include "RubicsCube.h"
#include <iostream>

using namespace std;

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
	
	RubicsCube::RubicsCube(void)
	{
		direction = 1;
		factor = 1;
		rotCount = 0;
		lastAxis = 3;
	}

	RubicsCube::RubicsCube(vec3 position,float angle,float hwRelation,float near, float far) : Scene(position,angle,hwRelation,near,far)
	{
		direction = 1;
		factor = 1;
		rotCount = 0;
		lastAxis = 3;

		
	}

	RubicsCube::~RubicsCube(void)
	{
	}

	void RubicsCube::init(Vertex *vertices,unsigned int *indices,int verticesSize,int indicesSize)
	{
		addShape(vertices, verticesSize, indices, indicesSize,"./res/textures/plane.png");
		for(int i=1;i<squarsNum*squarsNum*squarsNum;i++)
		{
			addShape(0);
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
					pickedShape = (i + squarsNum/2)*squarsNum*squarsNum + (j + squarsNum/2)*squarsNum + k + squarsNum/2;
					if(j!=0)
						shapeTransformation(1,j*2);
					if(i != 0)
						shapeTransformation(0,i*2);
					if(k != 0)
						shapeTransformation(2,k*2);
					if(squarsNum % 2==0)
					{
						shapeTransformation(0,1);
						shapeTransformation(1,1);
						shapeTransformation(2,1);
					}
				}
			}
		}
		pickedShape = -1;
	}

	void RubicsCube::addShape(int type)
	{
		indxArray.push_back(indxArray.size());
		__super::addShape(type);
	}

	void RubicsCube::addShape(const std::string& fileName)
	{
		indxArray.push_back(indxArray.size());
		__super::addShape(fileName);
	}

	void RubicsCube::addShape(const std::string& fileName,const std::string& textureFileName)
	{
		indxArray.push_back(indxArray.size());
		__super::addShape(fileName,textureFileName);
	}

	void RubicsCube::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
	{
		indxArray.push_back(indxArray.size());
		__super::addShape(vertices,numVertices,indices,numIndices);
	}
	
	void RubicsCube::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName)
	{
		indxArray.push_back(indxArray.size());
		__super::addShape(vertices,numVertices,indices,numIndices,textureFlieName);
	}


	void RubicsCube::rotateWall(int type,float amt)
	{
		
		int savePick = pickedShape;
	    int i,j,k;
		//printArr(indxArray);
		if(lastAxis == type || (lastAxis != type && rotCount%factor==0))
		{
			rotCount++;
			lastAxis = type;
			switch (type)
			{
				case -3:
				case 3:
					if(type<0)
						i = sign(type)*(squarsNum/2);
					else i = squarsNum % 2;
					for(int j= -squarsNum + squarsNum/2 + squarsNum%2; j <=squarsNum - squarsNum/2 -1 ; j++)
					{
						for(int k = -squarsNum + squarsNum/2 + squarsNum%2;k<=squarsNum - squarsNum/2 -1;k++)
						{
							pickedShape = indxArray[(i + 1)*squarsNum*squarsNum + (j + 1)*squarsNum + k + 1];
							shapeTransformation(abs(type),amt*direction/factor);
						}
					}
				break; 
				case -4:
				case 4:
					if(type<0)
						j = sign(type)*(squarsNum/2);
					else j = squarsNum % 2; 

					for(int i= -squarsNum + squarsNum/2 + squarsNum%2; i <=squarsNum - squarsNum/2 -1 ; i++)
					{
						for(int k = -squarsNum + squarsNum/2 + squarsNum%2;k<=squarsNum - squarsNum/2 -1;k++)
						{
							pickedShape = indxArray[(i + 1)*squarsNum*squarsNum + (j + 1)*squarsNum + k + 1];
							shapeTransformation(abs(type),amt*direction/factor);
						}
					}
				break;
				case -5:
				case 5:
					if(type<0)
						k = sign(type)*(squarsNum/2);
					else k = squarsNum % 2;
					// k = sign(type) * (squarsNum % 2);
					
					for(int i =-squarsNum + squarsNum/2 + squarsNum%2 ; i <= squarsNum - squarsNum/2 - 1;i++)
					{
						for(int j= -squarsNum + squarsNum/2 + squarsNum%2; j <=squarsNum - squarsNum/2 -1 ; j++)
						{
							pickedShape = indxArray[(i + 1)*squarsNum*squarsNum + (j + 1)*squarsNum + k + 1];
							shapeTransformation(abs(type),amt*direction/factor);
						}
					}
				break;
			default:
				break;
			}
			if(rotCount%factor == 0)
			{
				updateIndices(type);
				printArr(indxArray);
			}
		}
		else
			std::cout<<"can not rotate"<<std::endl;
		pickedShape = savePick;
	}

	void RubicsCube::updateIndices(int type)
	{
		int diffX,diffY,cell,tmp, base = squarsNum - 1;
		switch (type)
		{
		case -3:
		case 3:
			if(type<0)
				cell = 0;
			else
				cell = (sign(type) + (squarsNum % 2))*squarsNum*squarsNum;
			if(direction == -sign(type))
			{
			  diffX = 1;
			  diffY = squarsNum;
			}
			else
			{
			  diffX = squarsNum;
			  diffY = 1;
			}
			break;
		case -4:
		case 4:
			if(type<0)
				cell = 0;
			else
				cell = (sign(type) + (squarsNum % 2))*squarsNum;
			if(direction == -sign(type))
			{
			  diffX = squarsNum*squarsNum;
			  diffY = 1;
			}
			else
			{
			  diffX = 1;
			  diffY = squarsNum*squarsNum;
			}
			break;
		case -5:
		case 5:
			if(type<0)
				cell = 0;
			else
				cell = (sign(type) + (squarsNum % 2));
			if(direction == sign(type))
			{
			  diffX = squarsNum*squarsNum;
			  diffY = squarsNum;
			}
			else
			{
			  diffX = squarsNum;
			  diffY = squarsNum*squarsNum;
			}
			
			break;
		default:
			break;
		}
		//corners
			tmp = indxArray[cell];
			indxArray[cell]= indxArray[cell + base*diffX];
			indxArray[cell + base*diffX]= indxArray[cell + base*diffX + base*diffY];
			indxArray[cell + base*diffX + base*diffY]= indxArray[cell  +  base*diffY];
			indxArray[cell + base*diffY] = tmp;
			//edges:
			if(squarsNum % 2==1)
			{
				cell+=diffX; 
				tmp = indxArray[cell];
				indxArray[cell] = indxArray[cell + diffX + diffY];
				indxArray[cell + diffX + diffY]= indxArray[cell + 2*diffY];
				indxArray[cell + 2*diffY]= indxArray[cell + diffY - diffX];
				indxArray[cell + diffY - diffX] = tmp;
			}
	}
