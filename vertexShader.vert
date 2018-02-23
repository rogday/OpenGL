#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 ourColor;

out vec3 aCol;
out vec4 position;

uniform float offset;

void main(){
	vec3 pos = aPos;

	float cos = sqrt(1 - pow(offset, 2));
	float sin = offset;

	pos.xy = mat2(cos, -sin, sin, cos)*pos.xy;

	position = gl_Position = vec4(pos.xyz, 1.0);
	aCol = ourColor;
}