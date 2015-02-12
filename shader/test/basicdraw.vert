#version 450 core

layout(location = 0) in vec3 pos;

out vec3 color;

uniform vec3 col;

void main() {
	gl_Position = vec4(pos, 1.f);
	color = col;
}
