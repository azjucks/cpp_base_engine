#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

//uniform vec3 ourColor;
uniform sampler2D ourTexture;


void main()
{
   FragColor = texture(ourTexture, TexCoord);
   //FragColor = ourColor ;
}