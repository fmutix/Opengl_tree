#version 330 core

uniform int renderStyle;
uniform vec3 cameraPos;

uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float ambient;
uniform vec3 diffusePos;
uniform float specularIntensity;
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

vec2 sphericalMapping(vec3 cartesianCoord){
	float m = 2 * length(cartesianCoord);
	float u = cartesianCoord.x / m + 0.5f;
	float v = cartesianCoord.y / m + 0.5f;
	return vec2(u, v);
}

vec2 latitudeMapping(vec3 cartesianCoord){
	float u = (atan(cartesianCoord.x / cartesianCoord.z) + M_PI) / (2 * M_PI);
	float v = (asin(cartesianCoord.y) + M_PI / 2) / M_PI;
	return vec2(u, v);
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

	vec3 sphericalCoord = getSphericalCoord(objPos)	;
	vec4 texColor = texture(tex, vec2(sphericalCoord.y, sphericalCoord.z));
	//texColor = texture(tex, latitudeMapping(objPos));

	if (renderStyle == 0) {
		color = vec4((ambient + diffuse + specular) * vec3(texColor), 1.0f);
		/*color = vec4((ambient + diffuse + specular) * objectColor, 1.0f);*/
	}
	else if (renderStyle == 1) {
		if (abs(dot(viewDir, norm)) < 0.2) {
			color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		else {
			color = vec4((ambient + diffuse + specular) * vec3(texColor), 1.0f);
		}
	}
}
