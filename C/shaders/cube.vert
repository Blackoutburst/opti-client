#version 410

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUv;
layout(location = 2) in float aFace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec2 uv;
out vec3 norm;
out vec3 tamer;
flat out float layer;

vec3 getColor(int index) {
	const vec3 normals[6] = vec3[6](
		vec3(1.0f, 0.0f, 0.0f), // RED
		vec3(0.0f, 0.0f, 1.0f), // GREEN
		vec3(0.0f, 0.0f, 1.0f), // BLUE
		vec3(0.0f, 1.0f, 1.0f), // CYAN
		vec3(1.0f, 1.0f, 0.0f), // YELLOW
		vec3(1.0f, 0.0f, 1.0f) // MAGENTA
	);
	return normals[index];
}

vec3 getNormal(int index) {
	const vec3 normals[6] = vec3[6](
		vec3(0.0f, 1.0f, 0.0f), // TOP
		vec3(0.0f, 0.0f, -1.0f), // FRONT
		vec3(0.0f, 0.0f, 1.0f), // BACK
		vec3(-1.0f, 0.0f, 0.0f), // LEFT
		vec3(1.0f, 0.0f, 0.0f), // RIGHT
		vec3(0.0f, -1.0f, 0.0f) // BOTTOM
	);
	return normals[index];
}

void main() {
	FragPos = aPos - 0.5f;
	norm = getNormal(int(aFace));
	uv = aUv;
	tamer = getColor(int(aFace));

	gl_Position = projection * view * model * vec4(FragPos, 1.0);
}

