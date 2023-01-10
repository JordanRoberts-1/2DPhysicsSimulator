#include <GL/glew.h>
#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

//create a new vertex Array object
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

//Destroy the vertex array object
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

//add a vertex buffer to this vertex array object
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind(); //first, bind this buffer
	vb.Bind(); //Bind the vertex buffer
	const auto& elements = layout.GetElements(); //Get all the elements from the layout
	unsigned int offset = 0; //This keeps track of where we are in the vertex buffer

	//Loop through all the elements in the layout
	for (size_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i); //enable the element
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset); //set the structure of the the current buffer layout
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type); //Add how many bytes we've moved from the given layout
	}
}

//Bind this vertex array object
void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

//Unbind this vertex array object
void VertexArray::Unbind() const
{
	glBindVertexArray(0);

}
