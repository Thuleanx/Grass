#version 330 core

in vec2 uv;

uniform sampler2D _MainTex;
uniform vec4 _MainTex_TexelSize;

uniform bool applyGrayscale = false;
uniform bool applySharpen = true;

out vec4 fragColor;

const float sharpenMatrix[9] = float[](-1, -1, -1, -1, 17, -1, -1, -1, -1); 
const float sharpenNormalizer = 1.0/9;

vec4 grayscale(vec4 col) {
	return vec4(vec3(col.r * 0.299 + col.g * 0.587 + col.b * 0.114), 1);
}

vec4 sharpen() {
	vec4 res = vec4(0);
	for (int du = -1; du <= 1; du++)
	for (int dv = -1; dv <= 1; dv++) {
		vec2 uvA = vec2(_MainTex_TexelSize.x * du, _MainTex_TexelSize.y * dv) + uv;
		res += texture(_MainTex, uvA) * sharpenMatrix[(dv+1) * 3 + (du + 1)];
	}
	return res * sharpenNormalizer;
}

void main()
{
	vec4 textureCol = mix(texture(_MainTex, uv), sharpen(), applySharpen?1:0);
    fragColor = mix(textureCol, grayscale(textureCol), applyGrayscale?1:0);
	fragColor.a = 1;
}
