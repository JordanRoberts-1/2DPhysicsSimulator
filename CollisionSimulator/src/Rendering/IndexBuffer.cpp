#pragma once
#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(unsigned int count)
{
	m_Count = count;
	glGenBuffers(1, &m_RendererID); //Create a buffer and set the ID to the above buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); //Bind the above buffer ID to an Array buffer. This is what is going to be drawn in the next draw call.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW); //Actually create and assign the data to the buffer
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); //Bind the above buffer ID to an Array buffer. This is what is going to be drawn in the next draw call.
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Bind the above buffer ID to an Array buffer. This is what is going to be drawn in the next draw call.
}

void IndexBuffer::SetBuffer(const unsigned int* data, uint32_t size)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
