#version 330 core
#include utils.compute

layout (location = 0) in vec3 posWSIn;
layout (location = 1) in vec3 normalWSIn;
layout (location = 2) in vec2 uvIn;
layout (location = 3) in vec2 planeUV;
layout (location = 4) in float idHash;
layout (location = 5) in float grassHeightIn;
layout (location = 6) in vec3 rootWS;

out vec4 posWS;
out vec4 normalWS;
out vec2 uv;
out float grassHeight;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float time;

uniform sampler2D windNoise;
uniform vec4 windNoise_TexelSize;
uniform vec4 _TexelSize;

vec2 windDirection = vec2(1,1);
float windFrequencyLo = 0.5;
float windFrequencyHi = 0.2;
float windAmplitude = 0.65*0.3;

float windMacroFrequency = 0.03;
float windMacroSpeed = 0.06;
float windMacroAmplitude = 0.25;

uniform sampler2D velocityBuffer;
uniform vec4 velocityBuffer_samplingScale;

uniform sampler2D hillMap;
uniform float hillHeightNoiseScale;
uniform float hillHeightMax;

uniform vec3 displacement;

void applyWind(inout vec4 posWS) {
	float wind = cos(time * ((idHash >= 0.5 ? windFrequencyHi : windFrequencyLo) * (1+grassHeightIn/2)) +  idHash*0.5f);
	wind = (wind*wind*windAmplitude)*uvIn.y*(1+grassHeightIn/2)*0.6;

	float windMacro = uvIn.y * texture(windNoise, planeUV * windMacroFrequency + 
		time * windMacroSpeed * normalize(windDirection)).r * windMacroAmplitude;
	
	posWS += (wind + windMacro) * vec4(windDirection.x, 0, windDirection.y, 0);
}

bool isOutSideUV(vec2 uv) {
	return uv.x < 0 || uv.y < 0 || uv.x > 1 || uv.y > 1;
}

void applyParting(inout vec4 posWS) {
	vec4 pivotPoint = vec4(rootWS,1);

	vec2 uv = rootWS.xz * velocityBuffer_samplingScale.zw + velocityBuffer_samplingScale.xy;
	if (isOutSideUV(uv)) return;

	vec2 away = texture(velocityBuffer, uv).xy * 2 - 1;
	float strength = length(away);

	posWS = strength > 0 ?
		rotateAroundAxis(cross( vec3(away.x,0,away.y), vec3(0,1,0)), strength * 0.6 * M_PI) * 
		(posWS - pivotPoint) 
		+ pivotPoint : posWS;
}

void main() {
	posWS = vec4(posWSIn + displacement, 1);
	applyWind(posWS);
	applyParting(posWS);
	posWS += vec4(0,1,0,0) * (texture(hillMap, rootWS.xz * hillHeightNoiseScale).x * hillHeightMax);

	normalWS = vec4(normalize(normalWSIn),0);
	gl_Position = projMatrix * viewMatrix * posWS;
	grassHeight = grassHeightIn;
	uv = uvIn;
}
