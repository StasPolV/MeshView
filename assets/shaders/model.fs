#version 450 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

uniform sampler2D texture0;
uniform vec3 viewPos;

out vec4 FragColor;

struct DirLight 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 direction;
};
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 viewDir, vec3 normal);

void main() 
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// Directional Light
	vec3 result = CalcDirLight(dirLight, viewDir, norm);

	FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 viewDir, vec3 normal) 
{
	vec3 lightDir = normalize(-light.direction);

	// ambient
	vec3 ambient = light.ambient * texture(texture0, TexCoords).rgb;

	// diffuse
	float diff = max(0.0f, dot(lightDir, normal));
	vec3 diffuse = diff * light.diffuse * texture(texture0, TexCoords).rgb;

	// specular
	float spec = 0.0f;
	if (diff > 0.0f) 
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(0.0f, dot(halfwayDir, normal)), 32.0f);
	}
	vec3 specular = spec * light.specular * 0.5f;

	return (ambient + diffuse + specular);
}
