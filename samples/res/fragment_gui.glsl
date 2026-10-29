#version 330 core
precision mediump float;
out vec4 color;
in vec2 TexCoord;
uniform vec4 colorTint;
uniform sampler2D textureUnit;
uniform int isText;
void main() {
	vec4 pixColor = texture(textureUnit, TexCoord);
	if (isText == 1) {
		float brightness = dot(pixColor.rgb, vec3(0.299, 0.587, 0.114));
	
		float mask = 1.0 - brightness;
		mask *= pixColor.a;
	
		if (mask < 0.05) {
			discard;
		}
		color = vec4(colorTint.rgb, colorTint.a * mask);
	} else {
		if (pixColor.a < 0.01) {
			discard;
		}
		color = pixColor * colorTint;
	}
}