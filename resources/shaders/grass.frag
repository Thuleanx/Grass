#version 330 core

in vec4 posWS;
in vec4 normalWS;

// out vec4 fragColor;
layout(location = 0) out vec4 fragColor;

uniform vec4 camPos;

uniform float ka;
uniform float kd;
uniform float ks;

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;

uniform float shininess;

struct Light {
	vec4 pos;
	vec4 dir;
	vec3 function;
	vec4 color;
	float penumbra;
	float angle;
};

uniform Light pointLights[8];
uniform Light dirLights[8];
uniform Light spotLights[8];

float attenuation(vec3 function, float dist) {
	return min(1.0, 1.0/(function[0] + function[1] * dist + function[2] * dist * dist));
}

float saturate(float a) {
	return clamp(a,0,1);
}

float falloff(vec4 lightDir, vec4 dirToLight, float lightAngle, float lightPenumbra) {
	float angle = abs(acos(dot(-dirToLight, lightDir)));
	float t = saturate((angle - lightAngle + lightPenumbra) / lightPenumbra);
	float falloff = -2*t*t*t + 3*t*t;
	return saturate(1-falloff); 
}

vec4 calculateColor(vec4 lightDir, vec4 camDir, vec4 normWS) {
	vec4 con = vec4(0);
	float difMult = saturate(dot(normWS, -lightDir));
	con += diffuse * kd * difMult;

	vec4 R = reflect(lightDir, normWS);
	float specMult = saturate(dot(R, camDir));
	con += ks * specular * (shininess > 0 ? pow(specMult, shininess) : specMult);

	return con;
}

void main() {
	fragColor = ka * ambient;

	vec4 positionWS = vec4(vec3(posWS), 1);

	vec4 camDir = normalize(camPos - positionWS);
	vec4 normWS = normalize(normalWS);

	// dirLight
	for (int i = 0; i < 8; i++) {
		vec4 lightDir = normalize(dirLights[i].dir);
		fragColor += calculateColor(lightDir, camDir, normWS) * dirLights[i].color;
	}
	// pointLight
	for (int i = 0; i < 8; i++) {
		vec4 lightDir = -normalize(pointLights[i].pos - positionWS);
		float dist = length(vec3(pointLights[i].pos) - vec3(positionWS));
		float att = attenuation(pointLights[i].function, dist);
		fragColor += calculateColor(lightDir, camDir, normWS) * pointLights[i].color * att;
	}
	// spotLight
	for (int i = 0; i < 8; i++) {
		vec4 lightIncident = -normalize(spotLights[i].pos - positionWS);
		float dist = length(vec3(spotLights[i].pos) - vec3(positionWS));
		float att = attenuation(spotLights[i].function, dist) * falloff(-lightIncident, normalize(spotLights[i].dir), 
			spotLights[i].angle, spotLights[i].penumbra);
		fragColor += calculateColor(lightIncident, camDir, normWS) * spotLights[i].color * att;
	}

	fragColor.a = 1;
}