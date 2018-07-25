#include "MovableGLM.h"
#include <stdio.h>

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

vec3 MovableGLM::findAxis(vec3 vec) const
{
	//float tmpX = x*rotateMat[indx][0] + y*rotateMat[indx][1] + z*rotateMat[indx][2];
	//float tmpY = x*rotateMat[indx][4] + y*rotateMat[indx][5] + z*rotateMat[indx][6];
	//float tmpZ = x*rotateMat[indx][8] + y*rotateMat[indx][9] + z*rotateMat[indx][10];
	//x = tmpX;
	//y = tmpY;
	//z = tmpZ;
	return vec3(transpose(rotateMat*makeRot())*vec4(vec, 1));
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
	rotateMat = mat4(1);     //rotation around general point or (0,0,0) of global coordinate
	scaleFactor = vec3(1);
}

mat4 MovableGLM::makeTransScale(mat4  prevTransformations) const
{
	return prevTransformations * makeTrans() * scale(mat4(1), scaleFactor);
}

mat4 MovableGLM::makeTransScale2(mat4  prevTransformations) const
{
	return prevTransformations * makeTrans2() * scale(mat4(1), scaleFactor);
}

void MovableGLM::copyTranslations(MovableGLM *mGLM)
{
	for (size_t i = 0; i < 2; i++)
	{
		translateMat[i] = mGLM->translateMat[i];
	}
	//TODO: update rotation 
}


mat4 MovableGLM::makeTrans(mat4 prevTransformations) const
{
	return prevTransformations * makeTrans();
}

mat4 MovableGLM::makeTrans() const
{
	return  translateMat[0] * rotateMat * makeRot() * translateMat[1];
}
mat4 MovableGLM::makeTrans2() const {
	return     translateMat[0] * rotateMat * makeRot();
}

void MovableGLM::myRotate(float ang, glm::vec3 vec, int indx)
{
	//ang = ang / 180.0*pi;

	if (indx >= 0 && indx <= 3)
	{
		alignedRot(indx, ang, 0);
	}
	else
	{
		//split(rotate(makeRot(),ang,vec),10);
		rotateMat = rotate(rotateMat, ang, vec);
	}
	//	printAngles(zAxis12);
	//else //general rotation
	//{

	//}
}
void MovableGLM::myRotate2(float ang, glm::vec3 vec)
{
	//ang = ang / 180.0*pi;
	glm::mat4 forword = glm::translate(glm::mat4(1), glm::vec3(0, 0, 1));
	glm::mat4 backworld = glm::translate(glm::mat4(1), glm::vec3(0, 0, -1));
	mat4 rotzf = glm::rotate(mat4(1), ang, vec);
	mat4  T = forword * rotzf * backworld;
	rotateMat = T * rotzf;//rotate(rotateMat, ang, vec);

}
void MovableGLM::myRevRotate() {
	//rotateMat = ReversRotateMat;
	//ReversRotateMat = glm::mat4(1);
	deRevers = false;
}
void MovableGLM::myTranslate(vec3 vec, int indx)
{
	glm::mat4 forword = glm::translate(glm::mat4(1), glm::vec3(0, 0, 1));
	glm::mat4 backworld = glm::translate(glm::mat4(1), glm::vec3(0, 0, -1));
	translateMat[indx] = translateMat[indx] * translate(forword*rotateMat*backworld, scaleFactor * vec);

	rotateMat = glm::mat4(1);

}
void MovableGLM::myTranslate2(vec3 vec, int indx) {
	translateMat[indx] = translateMat[indx] * translate(glm::mat4(1), scaleFactor * vec);
	rotateMat = glm::mat4(1);
}
void MovableGLM::myScale(vec3  vec)
{
	scaleFactor = scaleFactor * vec;
}

void MovableGLM::globalSystemRot(float ang, vec3 vec, int indx)
{
	//ang = ang / 180.0*pi;
	vec3 tmp = vec;
	if (indx >= 0 && indx <= 3)
	{
		alignedRot(indx, ang, 1);
	}
	else
	{
		//	split(rotate(makeRot(),ang,vec),10);
		rotateMat = rotate(mat4(1), ang, vec) * rotateMat;

	}
	//	printAngles(zAxis12);
	//	rotateMat[indx] = rotate(mat4(1),ang,tmp)*rotateMat[indx];
}

void MovableGLM::translateInSystem(const MovableGLM  &system, vec3 vec, int indx, bool toScale)
{
	vec3 tmp;
	if (toScale)
		tmp = system.findAxis(scaleFactor * vec);
	else
		tmp = system.findAxis(vec);
	translateMat[indx] = translate(translateMat[indx], tmp);
}

void MovableGLM::changeCenterOfRotation(vec3  Pvec)
{
	myTranslate(Pvec, 0);
	myTranslate(-Pvec, 1);
}

void MovableGLM::zeroTrans()
{
	translateMat[0] = mat4(1);
	translateMat[1] = mat4(1);
}
void MovableGLM::zeroRotate() {
	rotateMat = mat4(1);
	rotzf = glm::mat4(1);;
}
void MovableGLM::getTraslate(vec4  vec)
{
	mat4 mat = makeTrans();
	vec = mat[3];
}

//vec3 MovableGLM::getTipPos(mat4 &prevTransformations)
//{
//	return vec3(makeTrans(prevTransformations) * translateMat[1] *  scale(mat4(1), scaleFactor) * vec4(0,0,0,1));
//}
//
//
//vec3 MovableGLM::getRootPos(mat4 &prevTransformations)
//{
//	return vec3(makeTrans(prevTransformations) * transpose(translateMat[1]) *  scale(mat4(1), scaleFactor) * vec4(0,0,0,1));
//}
//
vec3 MovableGLM::getXdirection(glm::mat4 prevTransformations)
{
	return vec3(makeTrans(prevTransformations)*vec4(1, 0, 0, 0));
}
vec3 MovableGLM::getZdirection(glm::mat4 prevTransformations)
{
	return vec3(makeTrans(prevTransformations)*vec4(0, 0, 1, 0));
}

vec3 MovableGLM::getCenterOfRotation(glm::mat4 prevTransformations)
{
	return vec3(makeTrans(prevTransformations) * transpose(translateMat[1])*vec4(0, 0, 0, 1));
}

glm::vec3 MovableGLM::getPointInSystem(glm::mat4 prevTransformations, glm::vec3 point)
{
	return vec3(makeTransScale(prevTransformations) * vec4(point, 1));
}

glm::vec3 MovableGLM::getVectorInSystem(glm::mat4 prevTransformations, glm::vec3 vec)
{
	return vec3(makeTrans(prevTransformations) * vec4(vec, 0));
}