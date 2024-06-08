#version 450 core

struct Light
{
	bool enabled;
	bool directional;
	bool spotlight;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 attenuation;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;
	float shininess;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform Light lights[8];


uniform sampler2D ourTexture;


void main()
{	
	// Main pixel color values;
	vec3 ambient = {0, 0, 0};
	vec3 diffuse = {0, 0, 0};
	vec3 specular = {0, 0, 0};

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	for (int i = 0; i < 8; ++i)
	{
		if (lights[i].enabled)
		{
			vec3 lightDir = {0, 0, 0};
			float attenuation = 1.0f;

			if (lights[i].directional) // directional light
			{
				lightDir = normalize(lights[i].position);
			}
			else // point light
			{
				lightDir = normalize(lights[i].position - FragPos);

				// Attenuation
				float distance = length(lights[i].position - FragPos);
				attenuation = 
					1.0 / (lights[i].attenuation.x
						 + lights[i].attenuation.y * distance
						 + lights[i].attenuation.z * (distance * distance));
			}
		

			// Ambient
			vec3 ambientLocal = material.ambient * lights[i].ambient;

			// Diffuse

			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuseLocal = (diff * material.diffuse) * lights[i].diffuse;
			
			// Specular
			vec3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
			vec3 specularLocal = (spec * material.specular) * lights[i].specular;

			ambientLocal *= attenuation;
			diffuseLocal *= attenuation;
			specularLocal *= attenuation;

			ambient += ambientLocal;
			diffuse += diffuseLocal;
			specular += specularLocal;
		}
	}
	 
	vec3 result = (ambient + diffuse + specular);
	result += material.emissive;
	FragColor = texture(ourTexture, TexCoord) * vec4(result, 1.0);
}