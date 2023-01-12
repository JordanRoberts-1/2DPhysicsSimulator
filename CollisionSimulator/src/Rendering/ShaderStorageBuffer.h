#pragma once
#include <cstdint>

class ShaderStorageBuffer
{
public:
	ShaderStorageBuffer(uint32_t size);
	~ShaderStorageBuffer();

	void Bind() const;
	void Unbind() const;

	void SetBuffer(const float* data, uint32_t size);
private:
	unsigned int m_RendererID; //The id assigned when this vertex buffer is created
};

