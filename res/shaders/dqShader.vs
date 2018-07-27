#version 120
attribute vec3 weights;
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 color;

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying vec3 weights0;
uniform mat4 MVP;
uniform mat4 Normal;
uniform int index;
uniform vec4 transQuats[20];
uniform vec4 rotQuats[20];

void main()
{

	vec4 weightedRotQuat = vec4(1);
	vec4 weightedTransQuat = vec4(1);
	gl_Position = MVP * vec4(position, 1.0);
	if(index != 4 ){
		if(index == 0) {
			weightedRotQuat = 
		    rotQuats[index] * weights.y +
		    rotQuats[index + 1] * weights.z ;

		  	weightedTransQuat =
		    transQuats[index] * weights.y +
		    transQuats[index + 1] * weights.z;
		}
		if(index > 0) {
			weightedRotQuat =
		    rotQuats[index - 1] * weights.x +
		    rotQuats[index] * weights.y +
		    rotQuats[index + 1] * weights.z;

		  	weightedTransQuat =
		    transQuats[index - 1] * weights.x +
		    transQuats[index] * weights.y +
		    transQuats[index + 1] * weights.z;
		}

		weightedTransQuat.w = 0;
		weightedRotQuat.w = 0;

	  float xRot = weightedRotQuat[0];
	  float yRot = weightedRotQuat[1];
	  float zRot = weightedRotQuat[2];
	  float wRot = weightedRotQuat[3];

	  float xTrans = weightedTransQuat[0];
	  float yTrans = weightedTransQuat[1];
	  float zTrans = weightedTransQuat[2];
	  float wTrans = weightedTransQuat[3];

	  float rotQuatMagnitude = sqrt(xRot * xRot + yRot * yRot
	   + zRot * zRot + wRot * wRot);
	  float TransQuatMagnitude = sqrt(xTrans * xTrans + yTrans * yTrans
	   + zTrans * zTrans + wTrans * wTrans);
	  weightedRotQuat = weightedRotQuat / rotQuatMagnitude;
	  weightedTransQuat = weightedTransQuat / TransQuatMagnitude;

	  float xR = weightedRotQuat[0];
	  float yR = weightedRotQuat[1];
	  float zR = weightedRotQuat[2];
	  float wR = weightedRotQuat[3];
	  float xT = weightedTransQuat[0];
	  float yT = weightedTransQuat[1];
	  float zT = weightedTransQuat[2];
	  float wT = weightedTransQuat[3];
	 
	  float len = dot(weightedTransQuat,weightedTransQuat);


	mat4 M;
	float len2 = dot(weightedTransQuat, weightedTransQuat);
	float w = weightedTransQuat.x, x = weightedTransQuat.y, y = weightedTransQuat.z, z = weightedTransQuat.w;
	float t0 = weightedRotQuat.x, t1 = weightedRotQuat.y, t2 = weightedRotQuat.z, t3 = weightedRotQuat.w;
		
	M[0][0] = w*w + x*x - y*y - z*z; M[0][1] = 2*x*y - 2*w*z; M[0][2] = 2*x*z + 2*w*y;
	M[1][0] = 2*x*y + 2*w*z; M[1][1] = w*w + y*y - x*x - z*z; M[1][2] = 2*y*z - 2*w*x; 
	M[2][0] = 2*x*z - 2*w*y; M[2][1] = 2*y*z + 2*w*x; M[2][2] = w*w + z*z - x*x - y*y;
	
	M[0][3] = -2*t0*x + 2*w*t1 - 2*t2*z + 2*y*t3;
	M[1][3] = -2*t0*y + 2*t1*z - 2*x*t3 + 2*w*t2;
	M[2][3] = -2*t0*z + 2*x*t2 + 2*w*t3 - 2*t1*y;
	
	M[3][0] = 0; M[3][1] = 0; M[3][2] = 0; M[3][3] = 1;

	//M /= len2;
	
	 M /= 1.9;

		color0 =color ;
		texCoord0 = texCoord;
		weights0 = weights;
		if (index>0)
			gl_Position += M * vec4(position, 1.0);
  	}
 	vec3 normal0 = (Normal * vec4(normal, 0.0)).xyz;
	
	
}
