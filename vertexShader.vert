#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTextCoord;

out vec3 aCol;
out vec2 TextCoord;
out vec4 position;

uniform mat4 R;

void main(){
	position = gl_Position = R*vec4(aPos, 1.0);
	aCol = aColor;
	TextCoord = aTextCoord;
}