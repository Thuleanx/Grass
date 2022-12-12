#version 330 core

in vec4 posWS;
in vec4 normalWS;
in vec2 uv;
in float grassHeight;

// out vec4 fragColor;
layout(location = 0) out vec4 fragColor;

// GRASS
uniform vec4 grassColorTip;
uniform vec4 grassColorTop;
uniform vec4 grassColorBottom;
uniform vec4 grassColorAmbientOcclusion;

uniform vec4 grassColorOld;

float saturate(float a) {
	return clamp(a,0,1);
}

vec4 saturate(vec4 a) {
	return vec4(
		clamp(a.x,0,1),
		clamp(a.y,0,1),
		clamp(a.z,0,1),
		clamp(a.w,0,1)
	);
}

vec4 grassColor(vec4 tip, vec4 top, vec4 bot, vec4 ao) {
	return mix(ao, mix(bot,top, uv.y), uv.y * uv.y)
		+ mix(tip, vec4(0,0,0,1), 1 - uv.y*uv.y*uv.y);
}

vec4 blend(vec4 a, vec4 b) { return 1-(1-a)*(1-b); }

void main() {
	vec4 col = saturate(grassColor(grassColorTip, grassColorTop, grassColorBottom, grassColorAmbientOcclusion));
	vec4 colOld = blend(col, grassColorOld);
	fragColor = mix(col, colOld, grassHeight * grassHeight * uv.y * uv.y);
	fragColor.a = 1;
}
