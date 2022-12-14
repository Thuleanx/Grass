#version 330 core

in vec4 posWS;
in vec4 normalWS;

uniform vec4 playerColor;
out vec4 fragColor;

void main() {
	fragColor = playerColor;
	fragColor.a = 1;
}
