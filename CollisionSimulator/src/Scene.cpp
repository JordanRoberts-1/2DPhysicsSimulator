#include "Scene.h"

#include <iostream>

#include "ECS.h"

void SceneManager::CreateScene()
{
	for (size_t i = 0; i < 100; i++)
	{
		AddEntity(CompTags::Position | CompTags::Velocity | CompTags::Collider);
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
	AppData::positions[currentId] = glm::vec3((float)currentId) / 1000.0f;

	currentId++;
}
