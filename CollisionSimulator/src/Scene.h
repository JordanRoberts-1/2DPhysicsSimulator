#pragma once
#include <cstdint>
#include <glm/glm.hpp>

using Tag = uint32_t;

class SceneManager
{
public:
	void CreateScene();
	void AddEntity(Tag tag);
private:
	uint32_t currentId = 0;
};

