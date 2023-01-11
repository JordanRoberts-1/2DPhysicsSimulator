#include "Application.h"

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include "Window.h"
#include "Rendering/Renderer.h"

int Application::Start()
{
	std::unique_ptr<Window> window = Setup();

	if (!window)
	{
		return -1;
	}

	Renderer renderer;
	renderer.AllocateBuffers();

	Run(window.get(), renderer);
	Cleanup();

	return 0;
}

std::unique_ptr<Window> Application::Setup()
{
	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	std::unique_ptr<Window> window = std::make_unique<Window>("2D Physics Simulation", 960, 540);

	if (glewInit() != GLEW_OK)
	{
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window->getGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	ImGui::StyleColorsDark();

	return window;
}

void Application::Run(Window* window, Renderer renderer)
{
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window->getGLFWWindow()))
	{
		renderer.ClearRendering();

		renderer.Render(window);

		/* Swap front and back buffers */
		glfwSwapBuffers(window->getGLFWWindow());

		/* Poll for and process events */
		glfwPollEvents();
	}
}

void Application::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}
