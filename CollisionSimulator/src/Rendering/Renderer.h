#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "glm/glm.hpp"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ShaderStorageBuffer.h"

class Window;
using Entity = uint32_t;

struct Vertex
{
	glm::vec2 position;
	glm::vec4 color;
	float mvpIndex;
};

class Renderer
{
public:
	Renderer();
	void SetLayout();
	void Clear();
	void Draw(uint32_t numEntities);
	void RenderGeometry(Window* window);
	void CreateRenderingUI(double frametime);
	void Render(Window* window);
private:
	void SetBuffers(std::vector<unsigned int>& indices, std::vector<Vertex>& vertices, std::vector<glm::mat4>& mvpMatrices);
	glm::mat4 CreateMVP(const Entity& e, glm::mat4& proj, glm::mat4& view);
	std::array<Vertex, 4> CreateVertices(const Entity& e);
	std::vector<unsigned int> BuildIndexBuffer(int numQuads);
	void RenderUI();
private:
	std::unique_ptr<VertexBuffer> m_Vb;
	std::unique_ptr<IndexBuffer> m_Ib;
	std::unique_ptr<VertexArray> m_Vao;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<ShaderStorageBuffer> m_Ssbo;

	std::vector<Vertex> m_Vertices;
	std::vector<glm::mat4> m_MVPMatrices;
	std::vector<unsigned int> m_Indices;
};