#include "Input.h"

#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../Scene.h"
#include "../Window.h"
#include <ImGUI/imgui.h>

namespace Input
{
	void Input::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		AppData::cursorX = 2 * xpos - AppData::windowWidth;
		AppData::cursorY = 2 * -ypos + AppData::windowHeight;
	}

	void MouseClickedCallback(GLFWwindow* window, int button, int action, int mods)
	{
		ImGuiIO& io = ImGui::GetIO();

		//only use data if ImGUI doesn't need it
		if (!io.WantCaptureMouse)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				if (action == GLFW_PRESS)
				{
					s_MouseHeld = true;
				}
				else if (action == GLFW_RELEASE)
				{
					s_MouseHeld = false;
				}
			}
		}
	}
}
