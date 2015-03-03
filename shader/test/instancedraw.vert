#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

layout(std140) uniform ModelMatrixBuffer {
  mat4 ModelMatrix[2500];
};

out vec3 color;

uniform vec3 col;
uniform mat4 ViewProj;

void main() {
	gl_Position = ViewProj * ModelMatrix[gl_InstanceID] * vec4(pos, 1.f);
	color = norm;
}
