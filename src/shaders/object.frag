#version 330 core

uniform int renderStyle;
uniform vec3 cameraPos;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform int objectHasTex;

uniform float ambient;
uniform vec3 diffusePos;
uniform float specularIntensity;
uniform float maxDayNight;
uniform float dayNight;
uniform sampler2D tex;

in vec3 fragPos;
in vec3 objPos;
in vec3 fragNormal;

out vec4 color;

#define M_PI 3.1415926535897932384626433832795

vec3 getSphericalCoord(vec3 cartesianCoord){
	float rho = length(cartesianCoord);
	float phi = acos(cartesianCoord.z/rho);
	float theta = atan(cartesianCoord.y, cartesianCoord.x);
	return vec3(rho, phi, theta);
}

void main (void) {
	// Ambient
	vec3 ambient = ambient * lightColor;

	// Diffuse
	vec3 norm = normalize(fragNormal);
	vec3 lightDirection = normalize(diffusePos - fragPos);
	float diff = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = diff * lightColor;

	// Specular
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = spec * specularIntensity * lightColor;

	vec3 sphericalCoord = getSphericalCoord(objPos);
	vec4 texColor = texture(tex, vec2(sphericalCoord.y, sphericalCoord.z));
	//texColor = texture(tex, latitudeMapping(objPos));

	float tmp = -1.0 + dayNight / maxDayNight;
	vec3 dayFactor = vec3(tmp, tmp, tmp);
	vec3 objectTextureColor;
	if(objectHasTex == 1) {
		objectTextureColor = vec3(texColor);
	}else{
		objectTextureColor = objectColor;
	}

	if(renderStyle == 1 && abs(dot(viewDir, norm)) < 0.4){
		color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}else{
		if(dayNight > 0){
			color = vec4((ambient + diffuse) * objectTextureColor + specular, 1.0f);
		}else{
			color = vec4(ambient * objectTextureColor, 1.0f);
		}
	}
}
