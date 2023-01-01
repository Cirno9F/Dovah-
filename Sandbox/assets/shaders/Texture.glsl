//Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;

out vec2 v_Texcoord;		

uniform mat4 u_ViewProjection;		
uniform mat4 u_Transform;		//Model Matrix

void main()
{
	v_Texcoord = a_Texcoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;
	
in vec2 v_Texcoord;	

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_Texcoord);
}
