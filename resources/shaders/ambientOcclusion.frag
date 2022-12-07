#version 330 core

in vec2 uv;

uniform sampler2D _depthTex;
uniform sampler2D _normalVS;
uniform sampler2D _posVS;
uniform vec4 _TexelSize;

uniform sampler2D _noiseTexture;
uniform vec4 _noiseTexture_TexelSize;

out float ambientOcclusion;
// out vec4 ambientOcclusion;

const int KERNEL_SZ = 64;
uniform vec3 kernel[KERNEL_SZ];
uniform float radius = 0.5;

uniform float zNear;
uniform float zFar;
uniform vec2 nearPlaneSz;

uniform mat4 projectionMatrix;

vec3 getPosVS(vec2 uvPos) {
	return texture(_posVS, uvPos);
}

vec2 getUV(vec3 posVS) {
	vec4 uvPos = vec4(posVS, 1);
	uvPos = projectionMatrix * uvPos;
	uvPos.xyz /= uvPos.w;
	return uvPos.xy * 0.5 + 0.5;
}

void main() {
	float occlusion = 0;

	vec3 pos = getPosVS(uv);

	vec3 normal = normalize(texture(_normalVS, uv).xyz);
	vec3 randomRotation = normalize(texture(_noiseTexture, uv * _TexelSize.zw * _noiseTexture_TexelSize.xy ).xyz); //  

	vec3 tangent = normalize(randomRotation - normal * dot(normal, randomRotation)); // take perp with normal
	vec3 bitangent = cross(normal, tangent);
	mat3 TangentToVS = mat3(tangent, bitangent, normal);

	for (int i = 0; i < KERNEL_SZ; i++) {
		vec3 kernelPos = TangentToVS * kernel[i] * radius + pos;

		float kernelDepth = getPosVS(getUV(kernelPos)).z;

		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(pos.z - kernelDepth)); // gets rid of artifact on edges of objects
		occlusion += (kernelDepth >= kernelPos.z + 0.025 ? 1 : 0) * rangeCheck;
	}

	occlusion = 1 - (occlusion / KERNEL_SZ);
	// ambientOcclusion = vec4(vec3(occlusion), 1);
	// ambientOcclusion = (applyAmbientOcclusion ? occlusion : 1);
	ambientOcclusion = occlusion;
}
