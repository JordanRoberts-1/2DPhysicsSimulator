#pragma once

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_Filepath(filepath), m_RendererID(0)
{
	//Parse the given file for the shader code
	ShaderProgramSource source = ParseShader(filepath);

	//get the id of the program with the given shaders
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

	m_RendererID = shader;
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

//create a file, vertexShader is a string of the source code of the Vertex Shader, fragmentShader is the same but
//for the Fragment Shader. Returns a unique identifier to the shader that is created
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); //Create the program environement for the shaders
	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER); //Compile the vertex shader source code as a vertex shader
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER); //Compile the fragment shader source code as a fragment shader

	glAttachShader(program, vs); //Attach the shaders compiled above to the program
	glAttachShader(program, fs);

	glLinkProgram(program); //Link the program
	glValidateProgram(program); //Validate that it is linked

	glDeleteShader(vs); //delete the intermediate files created when compiling the shader
	glDeleteShader(fs);

	return program; //return the id assigned to the program
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type); //Create a new shader of the given type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER) in this case
	const char* src = source.c_str(); //Turn the source code into a plain C String
	glShaderSource(id, 1, &src, nullptr); //Set the source for the shader
	glCompileShader(id); //Compile the shader with the given id

	int result; //Store the result of the shader process
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	//If the shader failed to compile for some reason
	if (result == GL_FALSE)
	{
		//Output the error message
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failure to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id); //delete the shader
		return 0; //return a failure state
	}

	return id;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	//Create an input stream from the given file
	std::ifstream stream(filepath);

	if (!stream) throw std::invalid_argument("Error creating Shader");

	//Enum for the shader type, also used as the indices of the types of shaders
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	//continuously read in each line
	while (getline(stream, line))
	{
		//If we find #shader, read in the type
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX; //Set the current type to vertex shader
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT; //Set the current type to fragment shader
			}
		}
		else // if we don't find #shader on the line, then add the current line to the corrseponding shader struct
		{
			ss[(int)type] << line << '\n';
		}
	}

	//return a new ShaderProgramSource with the given sources that were read in
	return { ss[0].str(), ss[1].str() };
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4 mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}