#version 130

// color might mean vertice placement, maybe change?

in vec2 texCoord0;
varying vec3 normal0;  
varying vec3 color0;
varying vec3 weights0;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 lightColor;


const float lightPower = 40.0;
// vec3 (R,G,B)
const vec3 ambientColor = vec3(0.1, 0.2, 0.4);
const vec3 diffuseColor = vec3(0.2, 0.7, 0.5);
const vec3 specColor = vec3(0.8, 1.0, 1.0);
const float shininess = 16.0;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

void main()
{
    float distance = length(lightDirection);
    vec3 normal = normalize(normal0);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    distance = distance * distance;

    float lambertian = max(dot(lightDirection,normal), 0.0);
    float specular = 0.2;

    vec3 reflectDir = reflect(-lightDirection, normal);
      float specAngle = max(dot(reflectDir, lightDirection), 0.0);
      specular = pow(specAngle, shininess/4.0);

  vec3 colorLinear = ambientColor +
                     diffuseColor * lambertian * lightColor * lightPower / distance +
                     specColor * specular * lightColor * lightPower / distance;
  
  vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0/screenGamma));

  gl_FragColor = texture2D(sampler, texCoord0) * vec4(colorGammaCorrected, 1.0);
}
