#pragma once

class IndexBuffer {
private:
	unsigned int m_RendererID = 0; //The id assigned when the Buffer is created
	unsigned int m_Count; //The number of indices in this buffer
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
