 #pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define M_PI 3.14159265359

class EulerAngles
{
public:
	enum axis{zAxis1,xAxis1,zAxis2,zAxis12};
	EulerAngles();
	EulerAngles(float phif, float thetaf,float psif);
	~EulerAngles();

	void printAngles(int ang);
	void buildAngMatrix(int XorZ, float c, float s);
	void alignedRot(int XorZ,float angle,int mode);
	glm::mat4 makeRot() const;
	glm::mat4 phi, theta, psi;
	inline void operator=(const EulerAngles &other) {
		int x = 0;
	}
private:
	
	

};