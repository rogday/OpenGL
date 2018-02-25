#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTextCoord;

out vec3 aCol;
out vec2 TextCoord;
out vec4 position;

uniform float time;

void main(){
	vec3 pos = aPos;

	vec3 axis = normalize(vec3(1, 0, 1));
	vec4 q = vec4(sin(time/2)*axis, cos(time/2));

	mat4 R = mat4(q.wz, -q.y, q.x, -q.z, q.wxyy, -q.x, q.wz, -q.xyz, q.w)*
	mat4(q.wz, -q.yxz, q.wx, -q.y, q.y, -q.x, q.w, -q.z, q.xyzw);

	vec4 v = vec4(pos, 0);
	pos = (R*v).xyz;

	position = gl_Position = vec4(pos.xyz, 1.0);
	aCol = aColor;
	TextCoord = aTextCoord;
}