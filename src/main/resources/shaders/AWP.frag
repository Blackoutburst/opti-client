#version 410 core

uniform sampler2DArray text;
uniform sampler2D diffuseMap;
uniform float layer;

in vec2 uv;
out vec4 FragColor;

void main() {
    if (texture(diffuseMap, uv).a < 0.1) {
        discard;
    }

    if (
        texture(diffuseMap, uv).r < 100.0/255.0 &&
        texture(diffuseMap, uv).g < 100.0/255.0 &&
        texture(diffuseMap, uv).b < 100.0/255.0 &&
        texture(diffuseMap, uv).g > texture(diffuseMap, uv).r &&
        texture(diffuseMap, uv).g > texture(diffuseMap, uv).b
    ) {
        FragColor = texture(text, vec3(uv * 10.0, layer)) * vec4(vec3(texture(diffuseMap, uv).r * 5.0), 1.0);
        return;
    }


    FragColor = vec4(1.0) * texture(diffuseMap, uv);
    //FragColor = vec4(uv,0.0, 1.0);
}