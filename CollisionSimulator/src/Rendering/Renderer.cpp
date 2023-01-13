#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../ECS.h"
#include "../Window.h"
#include "VertexBufferLayout.h"

Renderer::Renderer()
{
	m_Vao = std::make_unique<VertexArray>();
	m_Vb = std::make_unique<VertexBuffer>(MAX_ENTS * 4);
	m_Ib = std::make_unique<IndexBuffer>(MAX_ENTS * 6);
	m_Shader = std::make_unique<Shader>("res/Shaders/Basic.glsl");

	m_Vao->Bind();
	m_Ssbo = std::make_unique<ShaderStorageBuffer>(MAX_ENTS * sizeof(glm::mat4));

	m_Vertices.reserve(MAX_ENTS * 4);
	m_MVPMatrices.reserve(MAX_ENTS);
	m_Indices.reserve(MAX_ENTS * 6);
}

void Renderer::SetLayout()
{
	VertexBufferLayout m_VbLayout = VertexBufferLayout();
	m_VbLayout.Push<float>(2);
	m_VbLayout.Push<float>(4);
	m_VbLayout.Push<float>(1);

	m_Vb->Bind();
	m_Vao->AddBuffer(*m_Vb, m_VbLayout);
}

void Renderer::Draw(uint32_t numEntities)
{
	//Bind all of the buffers and stuff
	m_Ssbo->Bind();
	m_Shader->Bind();
	m_Vao->Bind();
	m_Ib->Bind();

	//Draw the number of entities * 6 indices
	glDrawElements(GL_TRIANGLES, numEntities * 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Renderer::Render(Window* window)
{
	RenderGeometry(window);
	RenderUI();

	glfwSwapBuffers(window->getGLFWWindow());
}

void Renderer::RenderGeometry(Window* window)
{
	//Tags required for this system
	constexpr Tag rqdTags = CompTags::Position | CompTags::Scale | CompTags::Rotation | CompTags::Renderable;

	//The cpu side buffers get cleared for a new frame
	m_Vertices.clear();
	m_MVPMatrices.clear();
	m_Indices.clear();

	//Set up the View and Projection matrices
	glm::mat4 proj = glm::ortho(-window->getWindowWidth(), window->getWindowWidth(), -window->getWindowHeight(), window->getWindowHeight(), -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f));
	
	//Loop through each entity and add them to cpu-side vertex buffer
	int renderableCount = 0;
	for (Entity e = 0; e < AppData::numEntities; e++)
	{
		if ((AppData::tags[e] & rqdTags) != rqdTags) 
			continue;

		//Add the current MVP matrix to the cpu-side Shader Storage Buffer Object
		m_MVPMatrices.push_back(CreateMVP(e, proj, view));

		//Add the vertices
		std::array<Vertex, 4> objectVertices = CreateVertices(e);
		m_Vertices.insert(m_Vertices.begin(), objectVertices.begin(), objectVertices.end());

		renderableCount++;
	}

	//Build the cpu-side index buffer
	m_Indices = BuildIndexBuffer(renderableCount);

	//upload the data to the gpu-side buffers
	SetBuffers(m_Indices, m_Vertices, m_MVPMatrices);

	Draw(renderableCount);
}

void Renderer::CreateRenderingUI(double frametime)
{
	ImGui::Begin("Rendering Stats");
	ImGui::Text("Frametime: %fms", frametime);
	ImGui::Text("Number of vertices: %d", m_Vertices.size());
	ImGui::Text("Number of Indices: %d", m_Indices.size());
	ImGui::Text("Number of MVP matrices: %d", m_MVPMatrices.size());
	ImGui::End();
}

void Renderer::RenderUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::SetBuffers(std::vector<unsigned int>& indices, std::vector<Vertex>& vertices, std::vector<glm::mat4>& mvpMatrices)
{
	m_Ib->Bind();
	m_Ib->SetBuffer(&indices[0], indices.size());

	m_Vb->Bind();
	m_Vb->SetBuffer((float*)&vertices[0], vertices.size());

	m_Ssbo->Bind();
	m_Ssbo->SetBuffer((float*)&mvpMatrices[0], mvpMatrices.size() * sizeof(glm::mat4));
}

std::array<Vertex, 4> Renderer::CreateVertices(const Entity& e)
{
	std::array<Vertex, 4> result;
	result[0] = {
		glm::vec2(0.0f, 0.0f),
		AppData::renderables[e].color,
		(float)e };
	result[1] = {
		glm::vec2(1.0f, 0.0f),
		AppData::renderables[e].color,
		(float)e };
	result[2] = {
		glm::vec2(1.0f, 1.0f),
		AppData::renderables[e].color,
		(float)e };
	result[3] = {
		glm::vec2(0.0f, 1.0f),
		AppData::renderables[e].color,
		(float)e };
	return result;
}

glm::mat4 Renderer::CreateMVP(const Entity& e, glm::mat4& proj, glm::mat4& view)
{
	glm::vec2 pos = AppData::positions[e];
	glm::vec2 scale = AppData::scales[e];
	float rotation = AppData::rotations[e];

	glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 1.0f) - glm::vec3(scale.x/2.0f, scale.y/2.0f, 0.0f));
	glm::mat4 model = glm::scale(transMatrix, glm::vec3(scale, 1.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	return proj * view * model;
}

std::vector<unsigned int> Renderer::BuildIndexBuffer(int numQuads)
{
	std::vector<unsigned int> buffer;
	buffer.reserve(numQuads * 6);
	for (uint32_t i = 0; i < numQuads; i++)
	{
		buffer.emplace_back(i*4);
		buffer.emplace_back(i*4+1);
		buffer.emplace_back(i*4+2);

		buffer.emplace_back(i*4+2);
		buffer.emplace_back(i*4+3);
		buffer.emplace_back(i*4);
	}

	return buffer;
}
