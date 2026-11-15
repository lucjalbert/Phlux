#version 330 core

out vec4 outColor;

uniform bool bIsSelected;
uniform bool bIsWireframe;
uniform vec4 aColor;

void main()
{
	if (bIsSelected == true)
	{
		outColor = vec4(1.0, 0.65, 0.0, 1.0);
	}
	else if (bIsWireframe == true)
	{
		outColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
	outColor = aColor;
	}
}