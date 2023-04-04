#version 310 es
precision mediump float;

in vec2 v_texCoord;

uniform sampler2D s_texture1;
uniform vec4  Ambient;
uniform float Selected;

void main()	
{
	if(Selected == 1.0f)
	{	
		 gl_FragColor = texture2D( s_texture, v_texCoord )*Ambient;
	}
		else
		{
		gl_FragColor = texture2D( s_texture, v_texCoord )*vec4(0.2f,0.0f,0.0f,1.f);
		}
} 