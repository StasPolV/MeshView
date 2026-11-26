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

struct PointLight 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;

	float constant;
	float linear;
	float quadratic;
};
#define MAX_POINT_LIGHTS_COUNT 16
uniform int PointLightsCount;
uniform PointLight PointLights[MAX_POINT_LIGHTS_COUNT];

struct SpotLight 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 direction;
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	float cut_off;
	float outer_cut_off;
};
#define MAX_SPOT_LIGHTS_COUNT 16
uniform int SpotLightsCount;
uniform SpotLight SpotLights[MAX_SPOT_LIGHTS_COUNT];

vec3 CalcDirLight(DirLight light, vec3 viewDir, vec3 normal);
vec3 CalcPointLight(PointLight light, vec3 viewDir, vec3 normal);
vec3 CalcSpotLight(SpotLight light, vec3 viewDir, vec3 normal);

void main() 
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// Directional Light
	vec3 result = CalcDirLight(dirLight, viewDir, norm);

	// Point Lights
	for (int i = 0; i < min(PointLightsCount, MAX_POINT_LIGHTS_COUNT); ++i) 
	{
		result += CalcPointLight(PointLights[i], viewDir, norm);
	}

	// Spot Lights
	for (int i = 0; i < min(SpotLightsCount, MAX_SPOT_LIGHTS_COUNT); ++i) 
	{
		result += CalcSpotLight(SpotLights[i], viewDir, norm);
	}

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
		spec = pow(max(0.0f, dot(halfwayDir, normal)), 32.0f);
	}
	vec3 specular = spec * light.specular * 0.5f;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 viewDir, vec3 normal) 
{
	vec3 lightDir = normalize(light.position - FragPos);

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
		spec = pow(max(0.0f, dot(halfwayDir, normal)), 32.0f);
	}
	vec3 specular = spec * light.specular * 0.5f;

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + distance * light.linear + distance * distance * light.quadratic);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 viewDir, vec3 normal) 
{
	vec3 lightDir = normalize(light.position - FragPos);

	// intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

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
		spec = pow(max(0.0f, dot(halfwayDir, normal)), 32.0f);
	}
	vec3 specular = spec * light.specular * 0.5f;

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + distance * light.linear + distance * distance * light.quadratic);

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular); 
}
