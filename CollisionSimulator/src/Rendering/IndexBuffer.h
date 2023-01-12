#pragma once
#include <cstdint>

class IndexBuffer {
public:
	IndexBuffer(unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetBuffer(const unsigned int* data, uint32_t count);

	inline unsigned int GetCount() const { return m_Count; }
private:
	unsigned int m_RendererID = 0; //The id assigned when the Buffer is created
	unsigned int m_Count; //The number of indices in this buffer
};
