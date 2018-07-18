#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
    vec3 tmp = dot(-lightDirection, normal0) * color0 ;
	//gl_FragColor = texture2D(sampler, texCoord0) * clamp(vec4(tmp,1.0), 0.0, 1.0);
	//gl_FragColor = texture2D(sampler, texCoord0) ;
}
