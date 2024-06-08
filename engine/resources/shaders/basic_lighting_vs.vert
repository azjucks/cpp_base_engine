#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 viewProj;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
   TexCoord = texcoord;
   FragPos = vec3(model * vec4(pos, 1.0));
   Normal = mat3(transpose(inverse(model))) * norm;

   gl_Position = (viewProj * model) * vec4(pos, 1.0);
}