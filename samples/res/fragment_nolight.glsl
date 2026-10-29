#version 330 core
precision mediump float;
out vec4 color;
in vec2 TexCoord;
uniform sampler2D textureUnit;
void main() {
	if (alpha < 0.01) {
        discard;
    }
	color = texture(textureUnit, TexCoord);
}