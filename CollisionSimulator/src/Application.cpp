#include "Application.h"

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include "Window.h"
#include "Rendering/Renderer.h"
#include "Scene.h"
#include "Util/Debug.h"
#include "Util/Clock.h"

int Application::Start()
{
	std::unique_ptr<Window> window = Setup();

	if (!window)
	{
		return -1;
	}

	Renderer renderer;
	renderer.AllocateBuffers();

	SceneManager sc;
	sc.CreateScene();

	Run(window.get(), renderer);
	Cleanup();

	return 0;
}

std::unique_ptr<Window> Application::Setup()
{
	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	std::unique_ptr<Window> window = std::make_unique<Window>("2D Physics Simulation", 960, 540);

	glfwSwapInterval(0);

	if (glewInit())
	{
		return nullptr;
	}

	//Print out the version of OpenGL to the console
	std::cout << glGetString(GL_VERSION) << std::endl;

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);


	//ImGui::CreateContext();
	//ImGui_ImplGlfw_InitForOpenGL(window->getGLFWWindow(), true);
	//ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	//ImGui::StyleColorsDark();

	Debug::SetupDebug();

	return window;
}

void Application::Run(Window* window, Renderer renderer)
{
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window->getGLFWWindow()))
	{
		renderer.ClearRendering();
		glfwPollEvents();

		double curr = Clock::CurrTimeInMillis();
		double elapsed = curr - m_Prev;
		m_Prev = curr;
		m_Lag += elapsed;

		//std::cout << "Frametime: " << elapsed << "ms" << std::endl;

		//Keep constant update time regardless of rendering speed
		while (m_Lag >= MS_PER_UPDATE)
		{
			//Update();

			m_Lag -= MS_PER_UPDATE;
		}

		//UI::RenderScore();

		renderer.Render(window);
	}
}

void Application::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}
