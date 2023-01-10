#pragma once

#include <GL/glew.h>
#include <iostream>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	static void ClearRendering();
	static void RenderGeometry();
	static void RenderGUI();
	static void Render();
private:
	static void Clear();
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