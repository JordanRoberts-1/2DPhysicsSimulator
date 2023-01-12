#shader vertex
#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

out vec4 v_color;

void main()
{
	v_color = a_color;
	gl_Position = vec4(a_position.xyz, 1.0f);
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec4 v_color;

void main()
{
	//color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	color = v_color;
}