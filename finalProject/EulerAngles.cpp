
#include "EulerAngles.h"
#include <stdio.h>
#include <iostream>

using namespace glm;

static void printMat(const mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}

EulerAngles::EulerAngles()
{
	phi = glm::mat4(1);
	theta = glm::mat4(1);
	psi = glm::mat4(1);
}


EulerAngles::EulerAngles(float phif, float thetaf, float psif)
{
	EulerAngles();
	phi = glm::rotate(phi, phif, glm::vec3(0, 0, 1));
	theta = glm::rotate(theta, thetaf, glm::vec3(1, 0, 0));
	psi = glm::rotate(psi, psif, glm::vec3(0, 0, 1));

}


EulerAngles::~EulerAngles()
{

}

void EulerAngles::printAngles(int ang)
{
	switch (ang)
	{
	case zAxis1:
		printf("Phi matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", phi[j][i]);
			printf("\n");
		}
		break;
	case xAxis1:
		printf("Theta matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", theta[j][i]);
			printf("\n");
		}

		break;
	case zAxis2:
		printf("Psi matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", psi[j] [i]);
			printf("\n");
		}
		break;
	case zAxis12:
		printf("Phi matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", phi[j] [i]);
			printf("\n");
		}
		printf("Theta matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", theta[j][  i]);
			printf("\n");
		}
		printf("Psi matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", psi[j ][ i]);
			printf("\n");
		}
		break;
	case 5:
		glm::mat4 tmp = phi*theta*psi;
		printf("rotation matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", tmp[j][i]);
			printf("\n");
		}
		break;
	}
	
}

void EulerAngles::buildAngMatrix(int XorZ, float c, float s)
{
	switch (XorZ)
	{
	case zAxis1:
		phi[0][0] = c;
		phi[1][1] = c;
		phi[1][0] = -s;
		phi[0][1] = s;
		break;
	case xAxis1:
		theta[1][1] = c;
		theta[2][2] = c;
		theta[2][1] = -s;
		theta[1][2] = s;
		break;
	case zAxis2:
		psi[0][0] = c;
		psi[1][1] = c;
		psi[1][0] = s;
		psi[0][1] = -s;
		break;
	case zAxis12:
		phi[0][0] = c;
		phi[1][1] = c;
		phi[1][0] = -s;
		phi[0][1] = s;
		psi[0][0] = c;
		psi[1][1] = c;
		psi[1][0] = s;
		psi[0][1] = -s;
		break;
	}
}
//XorZ see enum
//angle - rotation amount
//mode - to multiply the new matrix before the existing matrix or after 
void EulerAngles::alignedRot(int XorZ,float angle,int mode )
{
	float ang = angle;
	switch (XorZ)
	{
	case zAxis1:
		if(mode == 0)
			phi =  rotate(phi,ang,vec3(0,0,1));
		else 
			phi = rotate(mat4(1),ang,vec3(0,0,1)) * phi;
		break;
	case xAxis1:
		if(mode == 0)
			theta = rotate(theta,ang,vec3(1,0,0));
		else
			theta = rotate(mat4(1),ang,vec3(1,0,0)) * theta;
		break;
	case zAxis2:
		if(mode == 0)
			psi =  rotate(psi,ang,vec3(0,0,1));
		else
			psi = rotate(mat4(1),ang,vec3(0,0,1)) * psi;
		break;
	case zAxis12:
		if(mode == 0)
		{
			phi =  rotate(phi,ang,vec3(0,0,1));
			psi =  rotate(psi,-ang,vec3(0,0,1));
		}
		else
		{
			phi = rotate(mat4(1),ang,vec3(0,0,1)) * phi;
			psi = rotate(mat4(1),-ang,vec3(0,0,1)) * psi;
		}
		break;
	}
}
	
mat4 EulerAngles::makeRot() const
{
	return phi*theta*psi;
}