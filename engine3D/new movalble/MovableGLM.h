#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "EulerAngles.h"

using namespace glm;

class MovableGLM : public EulerAngles
{
private:
	mat4 translateMat[2];
	
	
	//vec3 findAxis2(vec3 vec, int indx);
protected:
	vec3 scaleFactor;
	vec3 findAxis(vec3 vec, int indx);
public:
	//vec3 findAxis(vec4 &vec, int indx);
	//vec3 findAxis2(vec4 &vec, int indx);
	MovableGLM();
	
	mat4 makeTransScale(mat4 &prevTransformations) const;
	mat4 makeTransScale() const;
	mat4 makeTrans(mat4 &prevTransformations) const;
	mat4 makeTrans() const;
	void myRotate(float ang, vec3 &vec, int indx);
	void myTranslate(vec3 &vec, int indx);
	void myScale(vec3 &vec);
	void globalSystemRot(float ang, vec3 &vec, int indx, int mode);
	void globalSystemTrans(vec3 &vec, int indx, int mode);
	void changeCenterOfRotation(vec3 &Pvec,int indx);
	void zeroTrans();
	void getTraslate(vec4 &vec);
	void copyTranslations(MovableGLM *mGLM);
	//float getTraslateZ() { return makeTrans()[3][2]; }
	//inline void rotateVec4(vec4 &vec) { vec = rotateMat[0] *vec; }
};