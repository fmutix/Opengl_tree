#version 330 core

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform vec3 objectModel;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VS_OUT {
	vec3 normal;
} vs_out;

void main() {
	gl_Position = projection * view * world * vec4(position + objectModel, 1.0f);

	vs_out.normal = normalize(
		mat3(projection * view * transpose(inverse(world))) * normal
	);
}
