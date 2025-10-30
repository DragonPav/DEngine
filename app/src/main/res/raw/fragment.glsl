#version 300 es
precision mediump float;
out vec4 color;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D textureUnit;
uniform float useLighting;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float specularStrength;
void main() {
	if (useLighting == 1.0f) {
        vec3 ambient = ambientStrength * lightColor;
        
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        vec3 viewDir = normalize(cameraPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
        vec3 specular = specularStrength * spec * lightColor;
        
        vec4 objectColor = texture(textureUnit, TexCoord);
        vec3 result = (ambient + diffuse + specular) * objectColor.rgb;
        color = vec4(result, 1.0f);
    } else {
        color = texture(textureUnit, TexCoord);
    }
}