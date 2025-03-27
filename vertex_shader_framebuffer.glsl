#version 330 core

layout (location = 0) in vec3 vertexPos;

out vec2 texCoord;

void main() {
	gl_Position = vec4(vertexPos, 1.0);
	texCoord = (vertexPos.xy + vec2(1, 1)) / 2.0;
}
