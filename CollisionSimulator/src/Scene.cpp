#include "Scene.h"

#include <iostream>

#include "ECS.h"
#include <ImGUI/imgui.h>

void SceneManager::CreateScene()
{
	for (size_t i = 0; i < 100; i++)
	{
		AddEntity(CompTags::Position | CompTags::Scale | CompTags::Rotation | 
			CompTags::Velocity | CompTags::Acceleration | CompTags::Collider | CompTags::Renderable);
	}
}

void SceneManager::AddEntity(Tag tag)
{
	if(AppData::numEntities >= AppData::tags.size())
	{
		std::cerr << "ENTITIES FULL, CAN'T ADD ANYMORE" << std::endl;
		return;
	}

	uint32_t currentIndex = AppData::numEntities;
	AppData::tags[currentIndex] = tag;
	AppData::positions[currentIndex] = glm::vec2(-480.0f) + glm::vec2(currentIndex * 5);
	AppData::scales[currentIndex] = glm::vec2(100.0f);
	AppData::rotations[currentIndex] = (float)currentIndex;

	AppData::renderables[currentIndex].color = glm::vec4(currentIndex /100.0f, currentIndex, currentIndex /100.0f, currentIndex);

	AppData::velocities[currentIndex] = glm::vec2(0.0f, -(float)currentIndex/100.0f);
	AppData::accelerations[currentIndex] = glm::vec2(0.0f, -(float)currentIndex / 1000.0f);

	AppData::numEntities++;
}

void SceneManager::CreateSceneUI()
{
	ImGui::Begin("Scene Statistics");
	ImGui::Text("MAX Entities: %d", MAX_ENTS);
	ImGui::Text("Number of Entities: %d", AppData::numEntities);
	if (ImGui::Button("Add Entity"))
	{
		AddEntity(CompTags::Position | CompTags::Scale | CompTags::Rotation | 
			CompTags::Velocity | CompTags::Collider | CompTags::Renderable);
	}
	ImGui::End();
}
