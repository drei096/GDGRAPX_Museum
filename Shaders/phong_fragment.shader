#version 330 core
uniform vec3 u_color;

in vec3 FragPos;
in vec2 UV;
in vec3 Normal;
in mat3 TBN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_secondary_diffuse;
uniform sampler2D texture_tertiary_diffuse;
uniform sampler2D texture_normal;
uniform vec3 u_light_pos;
uniform vec3 u_light_dir;
uniform vec3 u_camera_pos;
uniform vec3 u_ambient_color;
uniform float u_model_id;
uniform float u_tex_type;

out vec4 FragColor;

float attenuate(float value, float maximum)
{
	float clampedValue = min(value, maximum);
	return 1.0 / (pow(5 * clampedValue / maximum, 2) + 1);
}

float spotLightAttenuate(float value, float minimum, float maximum)
{
	return 1.0f - (clamp(value, minimum, maximum) - minimum) / (maximum - minimum);
}

float simple_attenuate(float value, float maximum)
{
	if (value > maximum)
	{
		value = 0.0f;
	}
	else
	{
		value = 1.0f;
	}
	return value;
}

//POINT LIGHT SHADER
void phongFragmentShader()
{
	//vertexColor = vec3(1.0, 1.0, 1.0);
	vec3 lightVector = normalize(u_light_pos - FragPos);

	float distance = length(u_light_pos - FragPos);
	float gradient = attenuate(distance, 30.0);

	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	float specularStrength = 5.0;

	vec3 viewDir = normalize(u_camera_pos - FragPos);
	vec3 reflectDir = reflect(-lightVector, Normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 10);

	vec3 specular = specularStrength * spec * lightColor;

	vec3 diffuse = vec3(max(dot(Normal, lightVector), 0.0)) * lightColor;
	vec3 ambient = u_ambient_color * lightColor;

	

	FragColor = vec4(ambient + (diffuse + specular) * gradient, 1.0) * texture(texture_diffuse, UV);
}

void phongAltFragmentShader()
{
	//vertexColor = vec3(1.0, 1.0, 1.0);
	vec3 lightVector = normalize(u_light_pos - FragPos);

	float distance = length(u_light_pos - FragPos);
	float gradient = attenuate(distance, 30.0);

	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	float specularStrength = 1.5;

	vec3 viewDir = normalize(u_camera_pos - FragPos);
	vec3 reflectDir = reflect(-lightVector, Normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 4);

	vec3 specular = specularStrength * spec * lightColor;

	vec3 diffuse = vec3(max(dot(Normal, lightVector), 0.0)) * lightColor;
	vec3 ambient = u_ambient_color * lightColor;



	FragColor = vec4(ambient + (diffuse + specular), 1.0) * texture(texture_diffuse, UV);
	//FragColor = texture(texture_diffuse, UV);
	//FragColor = vec3(u_color);
}

void basicFragmentShader()
{
	FragColor = texture(texture_diffuse, UV);
}

void directionalLightShader()
{

	
	//ALTERNATIVE DIR LIGHT
	float nDotL = dot(Normal, -u_light_dir);
	vec3 outColor = vec3(clamp(nDotL, 0, 1));
	//FragColor = vec4(outColor, 1) * texture(texture_diffuse, UV);
	

	/*
	//vertexColor = vec3(1.0, 1.0, 1.0);
	vec3 lightVector = normalize(u_light_dir);

	//float distance = length(u_light_pos - FragPos);
	//float gradient = attenuate(distance, 30.0);

	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	float specularStrength = 1.0;

	vec3 viewDir = normalize(u_camera_pos - FragPos);
	vec3 reflectDir = reflect(-lightVector, Normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 1);

	vec3 specular = specularStrength * spec * lightColor;

	float nDotL = max(dot(Normal, lightVector), 0.0);
	vec3 diffuse = vec3(nDotL) * lightColor;
	vec3 ambient = u_ambient_color * lightColor;
	*/

	if (u_tex_type == 1.0f)
		FragColor = vec4(outColor, 1.0) * texture(texture_diffuse, UV);
	else if (u_tex_type == 1.1f)
		FragColor = vec4(outColor, 1.0) * texture(texture_diffuse, UV) + texture(texture_secondary_diffuse, UV) * (1.0f - nDotL);
	
}

void alphaTestFragmentShader()
{
	vec4 texColor = texture(texture_diffuse, UV);
	if (texColor.a < 0.1)
	{
		discard;
	}
	FragColor = vec4(u_color, 1.0) * texColor;
}




void threeMultitexturingShader()
{
	vec3 lightVector = normalize(u_light_pos - FragPos);

	float distance = length(u_light_pos - FragPos);
	//float gradient = attenuate(distance, 30.0);

	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	float specularStrength = 1.5;

	vec3 viewDir = normalize(u_camera_pos - FragPos);
	vec3 reflectDir = reflect(-lightVector, Normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 4);

	vec3 specular = specularStrength * spec * lightColor;

	float nDotL = max(dot(Normal, lightVector), 0.0);

	vec3 diffuse = vec3(nDotL) * lightColor;
	vec3 ambient = u_ambient_color * lightColor;


	FragColor = vec4(ambient + (diffuse + specular), 1.0) * texture(texture_diffuse, UV) + texture(texture_secondary_diffuse, UV) + texture(texture_tertiary_diffuse, UV) * (1.0f - nDotL);
}

void normalFragmentShader()
{
	vec3 tbnNormal = texture(texture_normal, UV).rgb;

	tbnNormal = TBN * tbnNormal;

	float nDotL = dot(tbnNormal, -u_light_dir);
	vec3 outColor = vec3(clamp(nDotL, 0, 1));

	FragColor = vec4(outColor, 1.0) * texture(texture_diffuse, UV);
}

void spotlightFragmentShader()
{
	vec3 lightToSurface = normalize(u_light_pos - FragPos);
	vec3 lightDir = normalize(-u_light_dir);

	float distance = length(u_light_pos - FragPos);

	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	float specularStrength = 2.5;

	vec3 viewDir = normalize(u_camera_pos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 4);

	vec3 specular = specularStrength * spec * lightColor;

	vec3 diffuse = vec3(max(dot(Normal, lightDir), 0.0)) * lightColor;
	vec3 ambient = u_ambient_color * lightColor;

	float angle = abs(acos(dot(lightDir, lightToSurface)));
	float gradient = spotLightAttenuate(angle, 0.2f, 0.3f);

	FragColor = vec4(ambient + (diffuse + specular) * gradient, 1.0) * texture(texture_diffuse, UV);
}

void main()
{
	if (u_model_id == 1.0f)
	{
		basicFragmentShader();
	}
	else if (u_model_id == 1.1f)
	{
		phongFragmentShader();
	}
	else if (u_model_id == 1.2f)
	{
		directionalLightShader();
	}
	else if (u_model_id == 1.3f)
	{
		normalFragmentShader();
	}
	else if (u_model_id == 1.4f)
	{
		spotlightFragmentShader();
	}
	/*
	else if (u_model_id == 1.3f)
	{
		multitexturingShader();
	}
	else if (u_model_id == 1.4f)
	{
		threeMultitexturingShader();
	}
	*/
	
}

