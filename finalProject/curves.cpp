
#include <stdio.h>
#include <math.h>
#include "curves.h"

#define M_PI (glm::acos<float>(0)*2.0f)

Surface::Surface(int cylindricPartNum, int xRes, int yRes, int linkPosition)
{
	degree = 4;
	degree2 = 4;
	switch (linkPosition)
	{
	case all:
		subNum = cylindricPartNum + 2;
		break;
	case middle:
		subNum = cylindricPartNum;
		break;
	default:
		subNum = cylindricPartNum + 1;
		break;
	}
	shpType = linkPosition;
	xResolution = xRes;
	yResolution = yRes;
}

glm::vec3 Surface::calcNormal(float s, float t, std::vector <glm::vec3> &surface)
{
	glm::vec3 sumS(0, 0, 0);
	glm::vec3 sumT(0, 0, 0);
	bool noLoop1 = false, noLoop2 = false;
	if (s == 0)
	{
		sumS = surface[1] - surface[0];
		noLoop1 = true;
	}
	else if (s >= 1.0)
	{
		sumS = surface[degree2*degree - 1] - surface[degree2*degree - 2];
		noLoop1 = true;
	}
	if (t == 0)
	{
		sumT = surface[degree + 1] - surface[1];
		noLoop2 = true;
	}
	else if (t >= 1.0)
	{
		sumT = surface[degree2*degree - 2] - surface[degree2*degree - degree - 2];
		noLoop2 = true;
	}
	for (int i = 0; i < degree; i++)
	{
		for (int j = 0; j < degree2; j++)
		{
			float m1 = 1, m2 = 1;
			int c1 = 1, c2 = 1;
			float tmpS, tmpT;
			if (!noLoop1)
			{
				for (int k = 1; k < degree; k++)
				{

					if (k > i)
					{
						m1 = m1 * (1.0 - s);
						c1 = c1 * k;
						c2 = c2 * (k - i);
					}
					else
					{
						m2 = m2 * s;
					}
				}

				tmpS = (float)(c1 / c2)*m1*m2 / s * i - (float)(c1 / c2)*m1*m2 / (1.0 - s)*(degree - i - 1);
				sumS += tmpS * surface[i + j * degree];
			}
			if (!noLoop2)
			{
				m1 = 1;
				m2 = 1;
				c1 = 1;
				c2 = 1;
				for (int k = 1; k < degree2; k++)
				{
					if (k > j)
					{
						m1 = m1 * (1.0 - t);
						c1 = c1 * k;
						c2 = c2 * (k - j);
					}
					else
					{
						m2 = m2 * t;
					}
				}

				tmpT = (float)(c1 / c2)*m1*m2 / t * j - (float)(c1 / c2)*m1*m2 / (1.0 - t) *(degree2 - j - 1);

				sumT += tmpT * surface[i + j * degree];
			}
		}
	}
	glm::vec3 sum = glm::cross(sumS, sumT);
	return sum;
}

glm::vec3 Surface::calcNurbs(float s, float t, std::vector <glm::vec3> &surface)
{
	glm::vec3 sum(0, 0, 0);
	for (int i = 0; i < degree; i++)
	{
		for (int j = 0; j < degree2; j++)
		{
			float m1 = 1, m2 = 1;
			int c1 = 1, c2 = 1;
			float tmp;
			for (int k = 1; k < degree; k++)
			{

				if (k > i)
				{
					m1 = m1 * (1 - s);
					c1 = c1 * k;
					c2 = c2 * (k - i);
				}
				else
				{
					m2 = m2 * s;
				}
			}

			tmp = (float)(c1 / c2)*m1*m2;
			m1 = 1;
			m2 = 1;
			c1 = 1;
			c2 = 1;
			for (int k = 1; k < degree2; k++)
			{
				if (k > j)
				{
					m1 = m1 * (1 - t);
					c1 = c1 * k;
					c2 = c2 * (k - j);
				}
				else
				{
					m2 = m2 * t;
				}
			}
			tmp *= (float)(c1 / c2)*m1*m2;
			if (tmp>0)
				sum += tmp * surface[i + j * degree];
		}
	}
	return sum;
}

std::vector <glm::vec3> Surface::calcSurface(const std::vector <glm::vec3> &contour)
{
	//for (int i = 0; i < contour.size(); i++)
	//	printf("y(%d) = %f, ", i, contour[i].y);
	//printf("\n");

	std::vector <glm::vec3> contSurface;
	contSurface.clear();
	float r, R;
	float const alpha = M_PI / 2.0 / (float)(degree2 - 1);
	float const rathio = 1.0 / cos(alpha);

	for (int k = 0; k < subNum; k++)
	{
		for (int m = 0; m < 4; m++) //full ring
		{

			for (int i = 1, j = 0; j < degree2; i += degree - 1, j++) // square 
			{

				for (int l = 0; l < degree; l++) // part of ring
				{
					glm::vec3 p1, n1;

					if (l == 0 || l == degree - 1)
					{
						r = contour[(degree - 1) * k + l].y;
						if (j == 0 || j == degree2 - 1)
							p1 = glm::vec3(r*sin((float)((degree2 - 1) * m + j)*alpha), r*cos((float)((degree2 - 1) * m + j)*alpha), contour[(degree - 1) * k + l].x);
						else
						{
							R = r * rathio;
							p1 = glm::vec3(R*sin((float)((degree2 - 1) * m + j)*alpha), R*cos((float)((degree2 - 1) * m + j)*alpha), contour[(degree - 1) * k + l].x);
						}
						//n1 = glm::vec3(0, cos((float)(3 * m + j)*M_PI / 6.0), sin((float)(3 * m + j)*M_PI / 6.0));
					}
					else
					{
						if (j >0 && j < degree2 - 1)
							R = contour[(degree - 1) * k + l].y*rathio;
						else R = contour[(degree - 1) * k + l].y;

						p1 = glm::vec3(R*sin((float)((degree2 - 1) * m + j)*alpha), R*cos((float)((degree2 - 1) * m + j)*alpha), contour[(degree - 1) * k + l].x);
					}
					contSurface.push_back(p1);
					//normals.push_back(n1);
				}

			}
		}
	}
	return contSurface;
}

glm::vec3 Surface::calcWeight(float s, float t, std::vector <glm::vec3> &surface)
{
	float f1 = (1 - 4.0f*(1 - s)*s)*(1 - s) / 2.0f;
	float f2 = (2.0f*(1 - s)*s + 0.5f);
	float f3 = (1 - 4.0f*(1 - s)*s)*s / 2.0f;
	return glm::vec3(f1, f2, f3);
}

IndexedModel Surface::ToIndexedModel()
{
	float rathio = 1.0 / cos(M_PI / 2.0 / (float)(degree - 1));

	float  r = 1.0 / subNum, R = r * rathio;
	float  beta = M_PI / 2.0 / (degree - 1);
	IndexedModel result;
	std::vector <glm::vec3> contour;

	contour.clear();
	int i;
	glm::vec3 p2;
	//contour (ball)
	if (shpType == all || shpType == begin)
	{
		contour.push_back(glm::vec3(0, 0, 0));
		i = 2 - degree;
		for (; i < 0; i++)
		{
			contour.push_back(glm::vec3(R*sin((float)i*beta) + r, R*cos((float)i*beta), 0));
		}
	}
	else i = 0;

	float	l = r;
	if (shpType == middle || shpType == end)
		l = 0;
	p2 = glm::vec3(l + r * sin((float)i*beta), r*cos((float)i*beta), 0);
	contour.push_back(p2);


	int start = 2;
	if (shpType == middle)
		start = 0;
	else if (shpType != all)
		start = 1;

	for (int j = start; j < subNum; l += r, j++)
	{
		for (i = 1; i < degree; i++)
		{
			contour.push_back(glm::vec3(l + r / (float)(degree - 1)*i, r, 0));
		}

	}

	//i = 1;
	if (shpType == all || shpType == end)
	{
		for (i = 1; i < degree - 1; i++)
		{
			contour.push_back(glm::vec3(R*sin((float)i*beta) + l, R*cos((float)i*beta), 0));
		}
		p2 = glm::vec3(l + r * sin((float)i*beta), r*cos((float)i*beta), 0);

		contour.push_back(p2);
	}
	std::vector <glm::vec3> s;
	std::vector <glm::vec3> contSurface = calcSurface(contour);
	int indx = 0;
	for (int o = 0; o < subNum; o++)
	{
		for (int n = 0; n < 4; n++) // ring
		{
			s.clear();
			//controlPoints2D.push_back(glm::vec3((float)-k*4.0, 0, r));
			for (int m = 0; m < degree2; m++) // square
			{
				for (int k = 0; k < degree; k++) //p1, h1,h2,p2
				{
					int indx1 = k + m * degree + n * degree2*degree + degree2 * 4 * degree*o;
					s.push_back(contSurface[indx1]);
				}
			}

			for (i = 0; i < xResolution; i++)
			{
				for (int j = 0; j < yResolution; j++)
				{
					result.indices.push_back(indx++);
					result.positions.push_back(calcNurbs((float)i / xResolution, (float)j / yResolution, s));
					result.normals.push_back(calcNormal((float)i / xResolution, (float)j / yResolution, s));
					result.texCoords.push_back(glm::vec2((float)i / xResolution, (float)j / yResolution));
					result.colors.push_back(calcWeight((float)i / xResolution, (float)j / yResolution, s));
					result.weights.push_back(calcWeight((float)i / xResolution, (float)j / yResolution, s));

					result.indices.push_back(indx++);
					result.positions.push_back(calcNurbs((float)(i + 1) / xResolution, (float)j / yResolution, s));
					result.normals.push_back(calcNormal((float)(i + 1) / xResolution, (float)j / yResolution, s));
					result.texCoords.push_back(glm::vec2((float)(i + 1) / xResolution, (float)j / yResolution));
					result.colors.push_back(calcWeight((float)(i + 1) / xResolution, (float)j / yResolution, s));
					result.weights.push_back(calcWeight((float)(i + 1) / xResolution, (float)j / yResolution, s));


					result.indices.push_back(indx++);
					result.positions.push_back(calcNurbs((float)(i + 1) / xResolution, (float)(j + 1) / yResolution, s));
					result.normals.push_back(calcNormal((float)(i + 1) / xResolution, (float)(j + 1) / yResolution, s));
					result.texCoords.push_back(glm::vec2((float)(i + 1) / xResolution, (float)(j + 1) / yResolution));
					result.colors.push_back(calcWeight((float)(i + 1) / xResolution, (float)(j + 1) / yResolution, s));
					result.weights.push_back(calcWeight((float)(i + 1) / xResolution, (float)(j + 1) / yResolution, s));

					result.indices.push_back(indx++);
					result.positions.push_back(calcNurbs((float)i / xResolution, (float)j / yResolution, s));
					result.normals.push_back(calcNormal((float)i / xResolution, (float)j / yResolution, s));
					result.texCoords.push_back(glm::vec2((float)i / xResolution, (float)j / yResolution));
					result.colors.push_back(calcWeight((float)i / xResolution, (float)j / yResolution, s));
					result.weights.push_back(calcWeight((float)i / xResolution, (float)j / yResolution, s));

					result.indices.push_back(indx++);
					result.positions.push_back(calcNurbs((float)(i + 1) / xResolution, (float)(j + 1) / yResolution, s));
					result.normals.push_back(calcNormal((float)(i + 1) / xResolution, (float)(j + 1) / yResolution, s));
					result.texCoords.push_back(glm::vec2((float)(i + 1) / xResolution, (float)(j + 1) / yResolution));
					result.colors.push_back(calcWeight((float)(i + 1) / xResolution, (float)(j + 1) / yResolution, s));
					result.weights.push_back(calcWeight((float)(i + 1) / xResolution, (float)(j + 1) / yResolution, s));

					result.indices.push_back(indx++);
					result.positions.push_back(calcNurbs((float)i / xResolution, (float)(j + 1) / yResolution, s));
					result.normals.push_back(calcNormal((float)i / xResolution, (float)(j + 1) / yResolution, s));
					result.texCoords.push_back(glm::vec2((float)i / xResolution, (float)(j + 1) / yResolution));
					result.colors.push_back(calcWeight((float)i / xResolution, (float)(j + 1) / yResolution, s));
					result.weights.push_back(calcWeight((float)i / xResolution, (float)(j + 1) / yResolution, s));

				}
			}
		}
	}
	return result;
}




