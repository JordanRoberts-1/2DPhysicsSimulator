#include "Application.h"

#include "Window.h"

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int Application::Start()
{
	std::unique_ptr<Window> window = Setup();

	if (!window)
	{
		return -1;
	}

	Run(window->getGLFWWindow());
	Cleanup();
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

	return window;
}

void Application::Run(GLFWwindow* glfwWindow)
{
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(glfwWindow))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(glfwWindow);

		/* Poll for and process events */
		glfwPollEvents();
	}
}

void Application::Cleanup()
{
	glfwTerminate();
}
