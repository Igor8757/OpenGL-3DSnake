#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying float index0;

uniform sampler2D ourTexture1;  // added

uniform vec3 lightDirection;
uniform vec3 capOffSet;
uniform vec3 mediaColor;
uniform vec3 capColor;
void main()
{
    //vec3 tmp = dot(-lightDirection, normal0) * color0 ;
	
	gl_FragColor = texture(ourTexture1, texCoord0);
	if(index0 == 0.0 ){
		//if(color0.y<capOffSet.x){
			//gl_FragColor=vec4(vec3(capColor),1.0);	
		//}
	}
	//else if (index0 == 2.0){
		//if(color0.y>120.0){
		//	gl_FragColor=vec4(vec3(mediaColor),1.0);	
		//}
	//}
}
		

