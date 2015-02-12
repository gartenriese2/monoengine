#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

out vec3 color;

void main() {
	gl_Position = vec4(pos, 1.f);
	color = col;
}
