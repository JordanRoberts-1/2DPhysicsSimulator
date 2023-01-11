#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "glm/glm.hpp"
#include "VertexBuffer.h"
#include "../Window.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

//class VertexArray;
//class IndexBuffer;
//class Shader;
//class Window;

struct Vertex
{
	glm::vec3 position;
	glm::vec4 Color;
};

class Renderer
{
public:
	Renderer();
	void AllocateBuffers();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	void ClearRendering();
	void RenderGeometry(Window* window);
	void RenderGUI();
	void Render(Window* window);
private:
	VertexBuffer vb;
	IndexBuffer ib;
	VertexArray vao;
	Shader shader;
	std::vector<unsigned int> BuildIndexBuffer(int numQuads);
	void Clear();
private:
	static const uint32_t NUM_VERTICES = 16;
	static constexpr const float m_EntityVertices[NUM_VERTICES] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f
	};
	static constexpr const uint32_t QUAD_INDICES[6] = { 0, 1, 2, 2, 3, 0 };
};