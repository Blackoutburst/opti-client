#version 410

layout(location = 0) in int data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 chunkPos;

out vec3 FragPos;
out vec2 uv;
out vec3 norm;

flat out float layer;

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
	int U = (data >> 15) & 3;
	int V = (data >> 17) & 3;
	int N = (data >> 19) & 7;
	int T = (data >> 23) & 31;

	if (U == 2) U = 16;
	if (V == 2) V = 16;

	FragPos = vec3(X, Y, Z) + chunkPos;
	norm = getNormal(int(N));
	uv = vec2(U, V);
	layer = T;

	gl_Position = projection * view * model * vec4(FragPos, 1.0);
}

