#pragma once

class VertexBuffer {
private:
	unsigned int m_RendererID; //The id assigned when this vertex buffer is created
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
