#version 130

// color might mean vertice placement, maybe change?

in vec2 texCoord0;
varying vec3 normal0;  
varying vec3 color0;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
    vec3 tmp =  color0 ;
    
    vec3 L = normalize(gl_LightSource[0].position.xyz - color0);
    vec3 E = normalize(-color0);  
    vec3 R = normalize(-reflect(L,normal0));

    vec4 amb = gl_LightSource[0].ambient;

    vec4 diff = gl_FrontLightProduct[0].diffuse * max(dot(normal0,L), 0.0);
   	diff = clamp(diff, 0.0, 1.0);

   	vec4 spec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
   	spec = clamp(spec, 0.0, 1.0); 

   // write Total Color:  
   gl_FragColor = clamp(vec4(tmp,1.0), 0.0, 1.0) + amb + diff + spec;
	//gl_FragColor =  clamp(vec4(tmp,1.0), 0.0, 1.0);
}
