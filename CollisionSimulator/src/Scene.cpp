#include "Scene.h"

#include <iostream>

#include "ECS.h"

void SceneManager::CreateScene()
{
	for (size_t i = 0; i < 100; i++)
	{
		AddEntity(CompTags::Position | CompTags::Velocity | CompTags::Collider | CompTags::Renderable);
	}
}

void SceneManager::AddEntity(Tag tag)
{
	if(currentId >= AppData::tags.size())
	{
		std::cerr << "ENTITIES FULL, CAN'T ADD ANYMORE" << std::endl;
		return;
	}

	AppData::tags[currentId] = tag;
	AppData::positions[currentId] = glm::vec3(-1.0f) + glm::vec3(currentId/100.0f);
	AppData::renderables[currentId].color = glm::vec4(glm::vec3(0.0f + 0.01f * currentId), 1.0f);

	currentId++;
}
