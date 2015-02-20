#version 450 core

layout(location = 0) out vec4 outColor;

in vec2 Frag_UV;
in vec4 Frag_Color;

uniform sampler2D Texture;

void main() {
   outColor = Frag_Color * texture(Texture, Frag_UV.st);
}
