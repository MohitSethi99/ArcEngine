#type vertex
#version 450 core

layout (location = 0) in vec3 a_Position;

layout (location = 0) out vec3 v_TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_TexCoords = a_Position;
    gl_Position = u_Projection * mat4(mat3(u_View)) * vec4(a_Position, 1.0f);
}

#type fragment
#version 450 core

layout (location = 0) in vec3 v_TexCoords;

out vec4 fragColor;

uniform samplerCube u_EnvironmentMap;

void main()
{
	vec3 envColor = texture(u_EnvironmentMap, v_TexCoords).rgb;
	envColor = envColor / (envColor + vec3(1.0));
	envColor = pow(envColor, vec3(1.0/2.2));

    fragColor = vec4(envColor, 1.0);
}
