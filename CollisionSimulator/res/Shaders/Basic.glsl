#shader vertex
#version 460 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in float a_index;
layout(std430, binding = 0) buffer a_MVP {
	mat4 data_MVP[];
};

out vec4 v_color;

void main()
{
	int index = int(round(a_index));
	mat4 currMat = data_MVP[index];
	v_color = a_color;
	
	gl_Position = currMat * a_position;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_color;

void main()
{
	color = v_color;
}