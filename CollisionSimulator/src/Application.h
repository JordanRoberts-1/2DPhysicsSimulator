#pragma once
#include <memory>

#include "Rendering/Renderer.h"

class Window;
struct GLFWwindow;

class Application
{
public:
	Application();
	int Start();
	std::unique_ptr<Window> Setup();
	void Run(Window* window, Renderer* renderer);
	void Cleanup();
private:
	void SetupImGui(Window* window);
	void CreateContext();
	void Update();
private:
	double m_Prev, m_Lag;
	const double MS_PER_UPDATE = 8.8888888;
};

