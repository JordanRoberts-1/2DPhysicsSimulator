#pragma once

struct GLFWwindow;

class InputManager
{
public:
	static void Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
};
