#version 320 es
#define MAX_POINT_LIGHTS 1
precision mediump float;
out vec4 color;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;
uniform sampler2D textureUnit;
uniform sampler2D shadowMap;
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
float calcShadowFactor(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if (projCoords.z > 1.0) return 0.0;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}
vec3 calcDirLight(DirectionLight light, vec3 normal, vec3 viewDir, float shadowFactor) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 ambient = light.ambient * vec3(texture(textureUnit, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(textureUnit, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(textureUnit, TexCoord));
    return (ambient + (1.0 - shadowFactor) * (diffuse + specular));
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
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 lightDir = normalize(-dirLight.direction);
    float shadow = calcShadowFactor(FragPosLightSpace, norm, lightDir);
    vec3 result = calcDirLight(dirLight, norm, viewDir, shadow);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    color = vec4(result, 1.0f);
}