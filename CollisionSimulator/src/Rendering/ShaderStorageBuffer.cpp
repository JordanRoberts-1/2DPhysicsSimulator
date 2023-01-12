#include "ShaderStorageBuffer.h"

#include "Renderer.h"

ShaderStorageBuffer::ShaderStorageBuffer(uint32_t size)
{
	glCreateBuffers(1, &m_RendererID); //Create a new buffer with and set this VertexBuffer's id to that newly created buffer
	Bind(); //Bind this vertex buffer
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); //Actually create and assign the data to the buffer
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void ShaderStorageBuffer::Bind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, glGetProgramResourceIndex(
		1,
		GL_SHADER_STORAGE_BLOCK,
		"a_MVP"
	), m_RendererID);
}

void ShaderStorageBuffer::Unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
}

void ShaderStorageBuffer::SetBuffer(const float* data, uint32_t size)
{
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data); //to update partially
}
