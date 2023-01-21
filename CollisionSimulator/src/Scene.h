#pragma once
#include <cstdint>
#include <glm/glm.hpp>

using Tag = uint32_t;

namespace AppData
{
	inline double cursorX = 0.0, cursorY = 0.0;
}

class SceneManager
{
public:
	static void CreateScene();
	static int AddSquare();
	static int AddEntity(Tag tag);
	static void CreateSceneUI();
	static void SceneUpdate();
};

