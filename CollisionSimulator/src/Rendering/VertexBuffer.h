#pragma once
#include <cstdint>

class VertexBuffer {
private:
	unsigned int m_RendererID; //The id assigned when this vertex buffer is created
public:
	VertexBuffer(unsigned int count);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetBuffer(const float* data, uint32_t count);
};
