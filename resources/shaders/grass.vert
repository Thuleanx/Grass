#version 330 core

layout (location = 0) in vec3 posWSIn;
layout (location = 1) in vec3 normalWSIn;
layout (location = 2) in vec2 uvIn;

out vec4 posWS;
out vec4 normalWS;
out vec2 uv;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
	posWS = vec4(posWSIn,1);
	normalWS = vec4(normalize(normalWSIn),0);
	gl_Position = projMatrix * viewMatrix * posWS;
	uv = uvIn;
}
