#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 texPos;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

uniform float spriteSize;
uniform uvec2 spritePosition;

void main() {
	gl_Position = projection * camera * model * vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
	texCoord = vec2((spritePosition.x + texPos.x) * spriteSize, (spritePosition.y + texPos.y) * spriteSize);
}
