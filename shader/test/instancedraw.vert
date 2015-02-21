#version 450 core

layout(location = 0) in vec3 pos;

in int gl_InstanceID;

out vec3 color;

uniform vec3 col;
uniform uint width;
uniform mat4 MVP;

void main() {
	const int y = gl_InstanceID / int(width);
	const int x = gl_InstanceID % int(width);
	const float stepSize = 2.f / float(width);
	gl_Position = vec4(pos, 1.f);
	gl_Position.x += x * stepSize;
	gl_Position.y += y * stepSize;
	gl_Position *= MVP;
	color = col;
}
