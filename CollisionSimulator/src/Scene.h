#pragma once
#include <cstdint>
#include <glm/glm.hpp>

using Tag = uint32_t;

class SceneManager
{
public:
	static void CreateScene();
	static void AddEntity(Tag tag);
	static void CreateSceneUI();
};

