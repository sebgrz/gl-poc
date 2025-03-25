#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
	float TEX_SEGMENT = 1.0 / textureSize(tex, 1).x;
	vec4 rightTex = texture(tex, vec2(texCoord.x + TEX_SEGMENT * 2.0, texCoord.y));
	vec4 leftTex = texture(tex, vec2(texCoord.x - TEX_SEGMENT * 2.0, texCoord.y));
	vec4 topTex = texture(tex, vec2(texCoord.x, texCoord.y - TEX_SEGMENT * 2.0));
  vec4 texVec = texture(tex, texCoord);
	vec3 keyColor = vec3(0.0);
	if ( texVec.rgb == keyColor && (rightTex.rgb != keyColor || leftTex.rgb != keyColor || topTex.rgb != keyColor)) {
		fragColor = vec4(1.0);
	} else if (texVec.rgb == keyColor) {
		discard;
	} else {
		fragColor = texVec;
	}
}
