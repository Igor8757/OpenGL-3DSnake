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

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
	texCoord0 = texCoord;	
	weights0 = weights;
	color0 = color;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
}
