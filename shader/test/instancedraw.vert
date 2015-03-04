#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

layout(std140) uniform ModelMatrixBuffer {
	mat4 ModelMatrix[4];
};

out vec3 color;

uniform vec3 col;
uniform mat4 ViewProj;

void main() {
	gl_Position = ViewProj * ModelMatrix[0] * vec4(pos, 1.f);
	color = norm;
}
