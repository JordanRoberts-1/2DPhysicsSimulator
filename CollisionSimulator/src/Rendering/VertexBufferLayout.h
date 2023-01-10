#pragma once
#include <vector>
#include <GL/glew.h>

#include "Renderer.h"
#include "../Util/Debug.h"

struct VertexBufferElement
{
	unsigned int type; //The type that this is e.g GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_BYTE
	unsigned int count; //The number of individual vertices in this buffer element
	unsigned char normalized; //Whether the element is normalized (idk)

	//get the size of a OpenGL type
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

template<typename>
constexpr bool dependent_false = false;

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements; //The elements in the layout
	unsigned int m_Stride; //The length of the current element

public:
	VertexBufferLayout() : m_Stride(0) {}; //construct and initialize m_Stride to 0

	//Create a template for the various types that we push
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(dependent_false<T>);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE }); //Push the given count floats to the layout
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count; //Increment the stride by the correct number of bytes
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE }); //Push the given count of ints to the layout
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count; //Increment the stride by the correct number of bytes
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE }); //Push the given count of unsigned bytes to the layout
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count; //Increment the stride by the correct number of bytes
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; } // Get the elements
	inline unsigned int GetStride() const { return m_Stride; } //Get Stride 
};
