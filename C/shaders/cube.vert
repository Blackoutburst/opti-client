#version 410

layout(location = 0) in int data;

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
	int X = data & 31;
	int Y = (data >> 5) & 31;
	int Z = (data >> 10) & 31;
	int U = (data >> 15) & 31;
	int V = (data >> 20) & 31;
	int N = (data >> 25) & 7;

	FragPos = vec3(X, Y, Z);
	norm = getNormal(int(N));
	uv = vec2(U, V);
	tamer = getColor(int(N));

	gl_Position = projection * view * model * vec4(FragPos, 1.0);
}

