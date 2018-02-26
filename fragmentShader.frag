#version 330 core

in vec3 aCol;
in vec4 position;
in vec2 TextCoord;

out vec4 FragColor;

uniform sampler2D texture;

void main(){
	float d = 1 - 2*distance(TextCoord, vec2(0.5, 0.5))/sqrt(2.0);
	FragColor = mix(vec4(0, 0, 0, 1), texture2D(texture, TextCoord), d)+position;
}