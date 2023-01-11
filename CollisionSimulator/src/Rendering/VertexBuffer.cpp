#pragma once
#include "VertexBuffer.h"
#include "Renderer.h"

//Constructor
VertexBuffer::VertexBuffer(unsigned int size)
{
	glCreateBuffers(1, &m_RendererID); //Create a new buffer with and set this VertexBuffer's id to that newly created buffer
	Bind(); //Bind this vertex buffer
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); //Actually create and assign the data to the buffer
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); //Bind this VertexBuffer's id to opengl
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind this VertexBuffer
}

void VertexBuffer::SetBuffer(const float* data, uint32_t size)
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
