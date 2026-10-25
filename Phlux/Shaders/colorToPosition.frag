#version 330 core

in vec3 vertexColor;
in vec3 objectPosition;
out vec4 outColor;

uniform float time;

void main()
{
	outColor = mix(vec4(vertexColor, 1.0f), vec4(objectPosition, 1.0f), (sin(time) + 1.0f) * 0.5f);
}