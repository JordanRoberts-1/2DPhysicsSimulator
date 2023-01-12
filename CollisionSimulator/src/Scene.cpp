#include "Scene.h"

#include <iostream>

#include "ECS.h"
#include <ImGUI/imgui.h>

void SceneManager::CreateScene()
{
	for (size_t i = 0; i < 100; i++)
	{
		AddEntity(CompTags::Position | CompTags::Scale | CompTags::Rotation | 
			CompTags::Velocity | CompTags::Collider | CompTags::Renderable);
	}
}

void SceneManager::AddEntity(Tag tag)
{
	if(AppData::numEntities >= AppData::tags.size())
	{
		std::cerr << "ENTITIES FULL, CAN'T ADD ANYMORE" << std::endl;
		return;
	}

	AppData::tags[AppData::numEntities] = tag;
	AppData::positions[AppData::numEntities] = glm::vec2(-480.0f) + glm::vec2(AppData::numEntities * 5);
	AppData::scales[AppData::numEntities] = glm::vec2(100.0f);
	AppData::rotations[AppData::numEntities] = (float)AppData::numEntities;

	AppData::renderables[AppData::numEntities].color = glm::vec4(AppData::numEntities /100.0f, AppData::numEntities, AppData::numEntities /100.0f, AppData::numEntities);

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
