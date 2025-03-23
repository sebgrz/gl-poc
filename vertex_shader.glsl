#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 texPos;

out vec2 texCoord;

void main() {
	gl_Position = vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
	texCoord = vec2(texPos.x, texPos.y);
}
