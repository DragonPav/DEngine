#version 330 core
precision mediump float;
out vec4 color;
in vec2 TexCoord;
uniform sampler2D textureUnit;
void main() {
	color = texture(textureUnit, TexCoord);
}