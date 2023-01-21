#pragma once

struct GLFWwindow;

namespace Input
{
	void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseClickedCallback(GLFWwindow* window, int button, int action, int mods);

	inline bool s_MouseHeld = false;
}
