#version 330 core

in vec2 uv;

uniform sampler2D _MainTex;
uniform vec4 _MainTex_TexelSize;

out float fragColor;

const int SZ = 4;

void main()
{
	fragColor = 0;
	for (int du = -SZ; du <= SZ; du++)
	for (int dv = -SZ; dv <= SZ; dv++) {
		vec2 uvA = vec2(_MainTex_TexelSize.x * du, _MainTex_TexelSize.y * dv) + uv;
		fragColor += texture(_MainTex, uvA).r;
	}
	fragColor = fragColor / (1+2*SZ) / (1 + 2*SZ);
}
