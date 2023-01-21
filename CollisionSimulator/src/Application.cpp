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
#include "ECS.h"
#include "Util/Debug.h"
#include "Util/Clock.h"
#include "Util/Input.h"

Application::Application()
	: m_Prev(Clock::CurrTimeInMillis()), m_Lag(0.0)
{
}

int Application::Start()
{
	//Create Window
	std::unique_ptr<Window> window = Setup();
	if (!window)
		return -1;

	//Create Renderer and set the layout of the Vertex buffer
	Renderer renderer;
	renderer.SetLayout();

	//Create the scene
	SceneManager::CreateScene();

	//Set the background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Run(window.get(), &renderer);
	Cleanup();

	return 0;
}

std::unique_ptr<Window> Application::Setup()
{
	//Initialize glfw and check for error
	if (!glfwInit())
		return nullptr;

	//Set up the glfw version and other window hints
	CreateContext();

	std::unique_ptr<Window> window = std::make_unique<Window>("2D Physics Simulation");

	//Turn off v-sync
	glfwSwapInterval(0);

	//Initialize glew and check for error
	if (glewInit())
		return nullptr;

	//Print out the version of OpenGL to the console
	std::cout << glGetString(GL_VERSION) << std::endl;

	//Turn on alpha blending/transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setup input callbacks
	glfwSetCursorPosCallback(window->getGLFWWindow(), Input::CursorPositionCallback);
	glfwSetMouseButtonCallback(window->getGLFWWindow(), Input::MouseClickedCallback);

	SetupImGui(window.get());
	Debug::SetupDebug();


	return window;
}

void Application::SetupImGui(Window* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window->getGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	ImGui::StyleColorsDark();
}

void Application::CreateContext()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
}


void Application::Run(Window* window, Renderer* renderer)
{
	m_Prev = Clock::CurrTimeInMillis();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window->getGLFWWindow()))
	{
		renderer->Clear();
		glfwPollEvents();

		double curr = Clock::CurrTimeInMillis();
		double elapsed = curr - m_Prev;
		m_Prev = curr;
		m_Lag += elapsed;

		//Keep constant update time regardless of rendering speed
		while (m_Lag >= MS_PER_UPDATE)
		{
			Update();

			m_Lag -= MS_PER_UPDATE;
		}

		SceneManager::CreateSceneUI();
		renderer->CreateRenderingUI(elapsed);
		renderer->Render(window);
	}
}

void Application::Update()
{
	Systems::ProcessKinematics();
	Systems::ProcessCollisionDetection();
	SceneManager::SceneUpdate();
}

void Application::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}
