#version 330 core

in vec3 aCol;
in vec4 position;
out vec4 FragColor;
uniform float time;

void main(){
	FragColor = vec4(position.xyz, 1.0f);
}