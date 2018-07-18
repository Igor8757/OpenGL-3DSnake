#include "MovableGLM.h"
#include <stdio.h>

static void printMat(mat4 phi)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", phi[j][i]);
		printf("\n");
	}
}

vec3 MovableGLM::findAxis(vec3 vec, int indx)
{
	//float tmpX = x*rotateMat[indx][0] + y*rotateMat[indx][1] + z*rotateMat[indx][2];
	//float tmpY = x*rotateMat[indx][4] + y*rotateMat[indx][5] + z*rotateMat[indx][6];
	//float tmpZ = x*rotateMat[indx][8] + y*rotateMat[indx][9] + z*rotateMat[indx][10];
	//x = tmpX;
	//y = tmpY;
	//z = tmpZ;
	return vec3(transpose(makeRot())*vec4(vec,1));
}

//vec3 MovableGLM::findAxis2(vec3 vec, int indx)
//{
//	//float tmpX = x*rotateMat[indx][0] + y*rotateMat[indx][4] + z*rotateMat[indx][8];
//	//float tmpY = x*rotateMat[indx][1] + y*rotateMat[indx][5] + z*rotateMat[indx][9];
//	//float tmpZ = x*rotateMat[indx][2] + y*rotateMat[indx][6] + z*rotateMat[indx][10];
//	//x = tmpX;
//	//y = tmpY;
//	//z = tmpZ;
//	return vec3(vec4(vec, 1)*transpose(rotateMat[indx]));
//}

MovableGLM::MovableGLM()
{
	translateMat[0] = mat4(1);  //translation to general point
	translateMat[1] = mat4(1);  //translation to COM
	//rotateMat[0] = mat4(1);     //rotation around general point or (0,0,0) of global coordinate
	scaleFactor = vec3(1);
}

mat4 MovableGLM::makeTransScale(mat4 & prevTransformations) const
{
	return prevTransformations * makeTrans() * scale(mat4(1), scaleFactor);
}

void MovableGLM::copyTranslations(MovableGLM *mGLM)
{
	for (size_t i = 0; i < 2; i++)
	{
		translateMat[i] = mGLM->translateMat[i];
	}
	//TODO: update rotation 
}


mat4 MovableGLM::makeTrans(mat4 &prevTransformations) const
{
	return prevTransformations * makeTrans();
}

mat4 MovableGLM::makeTrans() const
{
	return  translateMat[0]  * makeRot() * translateMat[1] ;
}


void MovableGLM::myRotate(float ang, glm::vec3 &vec, int indx)
{
	ang = ang / 180.0*pi;
	
	if(indx >=0 && indx <=4)
	{
		alignedRot(indx,ang,0);
	}
	//else //general rotation
	//{

	//}
}

void MovableGLM::myTranslate(vec3 &vec, int indx)
{
	translateMat[indx] = translate(translateMat[indx], scaleFactor * vec ) ;
	//printMat(translateMat[indx]);
}

void MovableGLM::myScale(vec3 & vec)
{
	scaleFactor = vec;
}

void MovableGLM::globalSystemRot(float ang, vec3 &vec, int indx, int mode)
{
	ang = ang / 180.0*pi;
	vec3 tmp = vec;
	if(indx >=0 && indx <=4)
	{
		alignedRot(indx,ang,1);
	}

//	rotateMat[indx] = rotate(mat4(1),ang,tmp)*rotateMat[indx];
}

void MovableGLM::globalSystemTrans(vec3 &vec, int indx, int mode)
{
	vec3 tmp;
	if (mode == 0)
		tmp = findAxis(scaleFactor * vec, 0);
//	else
//		tmp = findAxis2(scaleFactor * vec, 1);
	translateMat[indx] = translate(translateMat[indx], tmp);	
}

void MovableGLM::changeCenterOfRotation(vec3 & Pvec,int indx)
{
	myTranslate(Pvec, indx );
	myTranslate(-Pvec, indx + 1);
}

void MovableGLM::zeroTrans()
{
	translateMat[0] = mat4(1);
	translateMat[1] = mat4(1);
}

void MovableGLM::getTraslate(vec4 & vec)
{
	mat4 mat = makeTrans();
	vec = mat[3];
}
