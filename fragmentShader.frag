#version 330 core

in vec3 aCol;
out vec4 FragColor;
uniform float time;

void main(){
	FragColor = vec4(aCol*(sin(time) + 1.0f)/2.0f, 1.0f);
}