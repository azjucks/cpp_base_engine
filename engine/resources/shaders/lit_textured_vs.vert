#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 viewProj;

out vec2 texCoord;

void main()
{
   gl_Position = (viewProj * model) * vec4(pos, 1.0);
   texCoord = texcoord;
}