#pragma once
#include <memory>
#include "Rendering/Renderer.h"

class Window;
struct GLFWwindow;

class Application
{
public:
	int Start();
	std::unique_ptr<Window> Setup();
	void Run(Window* window, Renderer renderer);
	void Cleanup();
};

