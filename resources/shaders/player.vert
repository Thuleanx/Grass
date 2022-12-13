#version 330 core

layout (location = 0) in vec3 posOS;
layout (location = 1) in vec3 normalOS;

out vec4 posWS;
out vec4 normalWS;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
	posWS = modelMatrix * vec4(posOS,1);
	normalWS = normalize(modelMatrix * vec4(normalOS,0));
	gl_Position = projMatrix * viewMatrix * posWS;
}
