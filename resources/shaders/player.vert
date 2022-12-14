#version 330 core

layout (location = 0) in vec3 posOS;
layout (location = 1) in vec3 normalOS;

out vec4 posWS;
out vec4 normalWS;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;


uniform sampler2D hillMap;
uniform float hillHeightNoiseScale;
uniform float hillHeightMax;
uniform vec3 objectPosition;

void main() {
	posWS = modelMatrix * vec4(posOS,1);

	posWS += vec4(0,1,0,0) * (texture(hillMap, objectPosition.xz * hillHeightNoiseScale).x * hillHeightMax);

	normalWS = normalize(modelMatrix * vec4(normalOS,0));
	gl_Position = projMatrix * viewMatrix * posWS;
}
