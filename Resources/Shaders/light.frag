#version 300 es
precision mediump float;

out mediump vec4 FragColor;

uniform mediump vec4 lightColor;

void main()
{
	FragColor = lightColor; //vec4(1.0f, 1.0f, 1.0f, 1.0f);
} 