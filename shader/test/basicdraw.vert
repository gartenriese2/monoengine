#version 450 core

layout(location = 0) in vec3 pos;

out vec3 color;

uniform vec3 col;
uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4(pos, 1.f);
	color = col;
}
