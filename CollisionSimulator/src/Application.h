#pragma once
#include <memory>

class Window;
class GLFWwindow;

class Application
{
public:
	static int Start();
private:
	static std::unique_ptr<Window> Setup();
	static void Run(GLFWwindow* window);
	static void Cleanup();
};
