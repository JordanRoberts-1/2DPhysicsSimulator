//#include "Input.h"
//
//#include "GL/glew.h"
//#include "GLFW/glfw3.h"
//#include "../ECS/PlayerComponent.h"
//
//void InputManager::Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
//	{
//
//		//send the keypress to every player component
//		//!Could be refactored with event/listener setup
//		for (auto& entity : SceneManager::GetInstance().GetObjects())
//		{
//			PlayerComponent* p = entity->GetComponent<PlayerComponent>(PLAYERCOMPONENT);
//			if (p)
//			{
//				p->HandleEvent();
//			}
//		}
//	}
//}
