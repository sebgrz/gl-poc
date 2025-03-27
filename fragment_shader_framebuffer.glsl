#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D renderedTexture;

void main() {
	fragColor = texture(renderedTexture, texCoord);
}
