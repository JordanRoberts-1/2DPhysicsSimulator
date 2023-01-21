#pragma once
#include <cstdint>
#include <glm/glm.hpp>

using Tag = uint32_t;

class SceneManager
{
public:
	static void CreateScene();
	static int AddSquare();
	static int AddEntity(Tag tag);
	static void CreateSceneUI();
};

