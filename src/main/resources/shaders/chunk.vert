#version 410

layout(location = 0) in int data;
layout(location = 1) in int offsetData;

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
	//XXXXX YYYYY ZZZZZ UUUUU VVVVV NNN TTT

	int X = data & 31;
	int Y = (data >> 5) & 31;
	int Z = (data >> 10) & 31;
	int U = (data >> 15) & 31;
	int V = (data >> 20) & 31;

	int XO = offsetData & 31;
	int YO = (offsetData >> 5) & 31;
	int ZO = (offsetData >> 10) & 31;
	int FACE = (offsetData >> 15) & 7;
	int T = (offsetData >> 18) & 31;
	vec3 offset = vec3(XO, YO, ZO);

	FragPos = vec3(X, Y, Z) + chunkPos;
	norm = getNormal(FACE);
	layer = T;

	vec3 pos = vec3(X, Y, Z);

	uv = vec2(U, V);

	if (FACE == 1) {
		uv = vec2(1-U, 1-V);
		pos.yz = pos.zy; pos.z--;
	}
	if (FACE == 2) {
		uv = vec2(1-U, 1-V);
		pos.zy = pos.yz;
	}
	if (FACE == 3) {
		uv = vec2(1-V, 1-U);
		pos.yx = pos.xy; pos.x--;
	}
	if (FACE == 4) {
		uv = vec2(1-V, 1-U);
		pos.xy = pos.yx;
	}

	if (FACE == 5) { pos.y--; }

	gl_Position = projection * view * vec4(pos + offset + chunkPos, 1.0);
}