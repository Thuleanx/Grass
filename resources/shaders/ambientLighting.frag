#version 330 core

in vec2 uv;

uniform sampler2D diffuseAndSpecularColor;
uniform sampler2D ambientColor;
uniform sampler2D ambientOcclusion;

uniform bool applyAmbientOcclusion;

out vec4 fragColor;

void main() {
	fragColor = texture(diffuseAndSpecularColor, uv);
	float occlusionFactor = applyAmbientOcclusion ? texture(ambientOcclusion, uv).r : 1;
	fragColor += occlusionFactor * texture(ambientColor, uv);
	fragColor.a = 1;
	// fragColor = vec4(vec3(occlusionFactor), 1);
}
