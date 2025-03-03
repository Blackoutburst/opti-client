#version 410

#define ambientStrength 0.5
#define specularStrength 0.1

in vec3 FragPos;
in vec2 uv;
in vec3 norm;

flat in float layer;

uniform sampler2DArray diffuseMap;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightPos;

uniform vec4 color;

out vec4 FragColor;

void main() {
    if (texture(diffuseMap, vec3(uv, layer)).a <= 0.5) {
        discard;
    }

    vec3 normal = normalize(norm);
    vec3 lightDir = vec3(0.4, 0.5, 0.25);
    //vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * color.xyz;

    FragColor = vec4(result, 1.0) * texture(diffuseMap, vec3(uv, layer));
}
