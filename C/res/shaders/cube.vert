#version 410

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUv;
layout(location = 2) in float aFace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;

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

void main() {
	vec3 FragPos = aPos - 0.5f;
	color = getColor(int(aFace));

	gl_Position = projection * view * model * vec4(FragPos, 1.0);
}
