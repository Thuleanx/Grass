#version 330 core

layout (location = 0) in vec3 posWSIn;
layout (location = 1) in vec3 normalWSIn;
layout (location = 2) in vec2 uvIn;
layout (location = 3) in vec2 planeUV;
layout (location = 4) in float idHash;
layout (location = 5) in float grassHeight;

out vec4 posWS;
out vec4 normalWS;
out vec2 uv;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float time;

vec2 windDirection = vec2(1,-1);
float windFrequencyLo = 0.5;
float windFrequencyHi = 0.2;
float windAmplitude = 0.65;

void main() {
	float wind = cos(time * ((idHash >= 0.8 ? windFrequencyHi : windFrequencyLo) * (1+grassHeight/2)) +  idHash*0.5f);
	wind = (wind*wind*windAmplitude)*uvIn.y*(1+grassHeight/2)*0.6;

	posWS = vec4(posWSIn + wind * vec3(windDirection.x, 0, windDirection.y),1);
	normalWS = vec4(normalize(normalWSIn),0);
	gl_Position = projMatrix * viewMatrix * posWS;
	uv = uvIn;
}
