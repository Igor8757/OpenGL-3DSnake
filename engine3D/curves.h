#include <vector>
#include <glm/glm.hpp>
#include "obj_loader.h"

class Surface
{
		int degree;
		int degree2; 
		int subNum;
		int xResolution;
		int yResolution;
		int shpType;
		glm::vec3 calcNormal(float s, float t, std::vector <glm::vec3> &surface);
		glm::vec3 calcNurbs(float s, float t, std::vector <glm::vec3> &surface);
		glm::vec3 calcWeight(float s, float t, std::vector <glm::vec3> &surface);
		std::vector <glm::vec3> calcSurface(const std::vector <glm::vec3> &contour);
		
		
public:
	enum parts{all,middle,begin,end};
	Surface(int cylindricPartNum,int xRes,int yRes,int linkPosition);
	IndexedModel ToIndexedModel();
};