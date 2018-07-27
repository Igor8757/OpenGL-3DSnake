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
	  float wR = 0;
	  float xT = weightedTransQuat[0];
	  float yT = weightedTransQuat[1];
	  float zT = weightedTransQuat[2];
	  float wT = 0;
	  float t0 = 2.0 * (-wT * xR + xT * wR - yT * zR + zT * yR);
	  float t1 = 2.0 * (-wT * yR + xT * zR + yT * wR - zT * xR);
	  float t2 = 2.0 * (-wT * zR - xT * yR + yT * xR + zT * wR);

	  float len = dot(weightedTransQuat,weightedTransQuat);
	  mat4 weightedJointMatrix = mat4(
	        1.0 - (2.0 * yR * yR) - (2.0 * zR * zR),
	        (2.0 * xR * yR) + (2.0 * wR * zR),
	        (2.0 * xR * zR) - (2.0 * wR * yR),
	        0,
	        (2.0 * xR * yR) - (2.0 * wR * zR),
	        1.0 - (2.0 * xR * xR) - (2.0 * zR * zR),
	        (2.0 * yR * zR) + (2.0 * wR * xR),
	        0,
	        (2.0 * xR * zR) + (2.0 * wR * yR),
	        (2.0 * yR * zR) - (2.0 * wR * xR),
	        1.0 - (2.0 * xR * xR) - (2.0 * yR * yR),
	        0,
	        t0,
	        t1,
	        t2,
	        1
	        );

	  	//weightedJointMatrix /= 3;

		color0 =color ;
		texCoord0 = texCoord;
		weights0 = weights;
		if (index>0)
			gl_Position +=  weightedJointMatrix * vec4(position, 1.0);
  	}
 	vec3 normal0 = (Normal * vec4(normal, 0.0)).xyz;
	
	
}
