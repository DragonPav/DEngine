#version 320 es
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;
void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = texCoord;
    FragPos = vec3(model * vec4(position, 1.0f));
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0f);
    Normal = mat3(transpose(inverse(model))) * normal;
}