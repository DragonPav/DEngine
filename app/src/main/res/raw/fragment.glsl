#version 300 es
#define MAX_POINT_LIGHTS 1
precision mediump float;
out vec4 color;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D textureUnit;
uniform float useLighting;
uniform vec3 cameraPos;
struct DirectionLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
vec3 calcDirLight(DirectionLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 ambient = light.ambient * vec3(texture(textureUnit, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(textureUnit, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(textureUnit, TexCoord));
    return (ambient + diffuse + specular);
}
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient = light.ambient * vec3(texture(textureUnit, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(textureUnit, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(textureUnit, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
void main() {
	if (useLighting == 1.0f) {
	    vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(cameraPos - FragPos);
        vec3 result = calcDirLight(dirLight, norm, viewDir);
        for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
            result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
        }
        color = vec4(result, 1.0f);
    } else {
        color = texture(textureUnit, TexCoord);
    }
}