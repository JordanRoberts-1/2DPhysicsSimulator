#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../ECS.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "../Window.h"



Renderer::Renderer()
	: vb(sizeof(Vertex) * 4000), ib(sizeof(int) * 6000), shader("res/Shaders/Basic.glsl")
{
}

void Renderer::AllocateBuffers()
{
	vao.Bind();

	VertexBufferLayout vbLayout = VertexBufferLayout();
	vbLayout.Push<float>(2);
	vbLayout.Push<float>(2);

	vao.AddBuffer(vb, vbLayout);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	//Bind all of the buffers and stuff
	//shader.Bind();
	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::RenderGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::Render(Window* window)
{
	Renderer::RenderGeometry(window);

	Renderer::RenderGUI();

	glfwSwapBuffers(window->getGLFWWindow());
}

void Renderer::RenderGeometry(Window* window)
{
	constexpr Tag rqdTags = CompTags::Position | CompTags::Renderable;

	std::vector<Vertex> vertices;

	glm::mat4 proj = glm::ortho(0.0f, window->getWindowWidth(), 0.0f, window->getWindowHeight(), -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

	int entityCount = 0;
	for (Entity e = 0; e < MAX_ENTS; e++)
	{
		if ((AppData::tags[e] & rqdTags) != rqdTags) { continue; }

		Vertex current = {
		AppData::positions[e],
		AppData::renderables[e].color };

		entityCount++;

		vertices.push_back(current);
	}

	std::vector<unsigned int> indices = BuildIndexBuffer(entityCount);

	ib.Bind();
	ib.SetBuffer(&indices[0], indices.size() * 6);

	vb.Bind();
	vb.SetBuffer((float*) & vertices[0], vertices.size() * sizeof(Vertex));

	Draw(vao, ib, shader);


	//SceneManager& sc = SceneManager::GetInstance();
	//const std::vector<std::unique_ptr<Entity>>& objects = sc.GetObjects();

	////Get Prepped for the MVP matrix
	//const Application& app = Application::GetInstance();

	//IndexBuffer ib(QUAD_INDICES, 6);


	////Loop through and render each object in a separate draw call
	//for (const std::unique_ptr<Entity>& object : objects)
	//{
	//	const float* vertices = m_EntityVertices;
	//	VertexBuffer vb(vertices, NUM_VERTICES * sizeof(float));

	//	vb.Bind();
	//	vao.AddBuffer(vb, vbLayout);

	//	TransformComponent* tc = object->GetTransform();
	//	glm::vec3 pos = tc->GetPosition();
	//	glm::vec3 scale = glm::vec3(tc->GetScaledSize(), 1.0f);

	//	//Setup the MVP matrix from each objects position, scale, rotation, etc...
	//	glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), pos);
	//	glm::mat4 model = glm::scale(transMatrix, scale);
	//	glm::mat4 mvp = proj * view * model;

	//	Renderer::Draw(vao, ib, *shader);
	//}
}

void Renderer::ClearRendering()
{
	/* Render here */
	Renderer::Clear();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

std::vector<unsigned int> Renderer::BuildIndexBuffer(int numQuads)
{
	std::vector<unsigned int> buffer;
	buffer.reserve(numQuads * 6);
	for (uint32_t i = 0; i < numQuads; i++)
	{
		buffer[i * 4] = i * 4;
		buffer[i * 4 + 1] = i * 4 + 1;
		buffer[i * 4 + 2] = i * 4 + 2;

		buffer[i * 4 + 2] = i * 4 + 2;
		buffer[i * 4 + 3] = i * 4 + 3;
		buffer[i * 4] = i * 4;
	}

	return buffer;
}