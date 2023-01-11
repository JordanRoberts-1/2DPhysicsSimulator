#pragma once
#include <cstdint>
#include <glm/glm.hpp>

#include "ECS.h"

class SceneManager
{
public:
	void CreateScene();
	void AddEntity(Tag tag);
private:
	uint32_t currentId = 0;
};

