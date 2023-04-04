#version 310 es
precision mediump float;
layout(location = 0) in vec3  a_position;
layout(location = 1) in vec2 a_texCoord;

uniform mat4 MVP;
out vec2 v_texCoord;


void main()
{
	gl_Position = MVP * vec4(a_position, 1);
		v_texCoord = a_texCoord;
}
