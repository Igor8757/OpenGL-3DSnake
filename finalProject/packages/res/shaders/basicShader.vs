#version 120

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 color;

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying float index0;

uniform mat4 MVP;
uniform mat4 Normal;

uniform float index;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
	
	texCoord0 = texCoord;
	if (index == 0.0){
		//texCoord0[0]=0.25+texCoord0[0];
		texCoord0[1]=1-texCoord0[1];
	}
	else if(index == 2.0){
		texCoord0[1]=1-texCoord0[1];
	}
	color0 = position;
	index0= index;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
}
