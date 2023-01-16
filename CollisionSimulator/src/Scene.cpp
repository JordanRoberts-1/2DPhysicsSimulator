#include "Scene.h"

#include <iostream>

#include "ECS.h"
#include <ImGUI/imgui.h>

void SceneManager::CreateScene()
{
	uint32_t startingNumEntities = AppData::numEntities;
	for (size_t i = startingNumEntities; i < startingNumEntities + 2; i++)
	{
		AddEntity(CompTags::TransformTagShortcut | 
			CompTags::Collider | CompTags::Renderable | CompTags::Rigidbody);
	}
}

void SceneManager::AddEntity(Tag tag)
{
	if(AppData::numEntities >= MAX_ENTS)
	{
		return;
	}

	uint32_t currentIndex = AppData::numEntities;
	AppData::tags[currentIndex] = tag;
	AppData::positions[currentIndex] = glm::vec2(0.0f, -300.0f + currentIndex * 350.0f);
	AppData::scales[currentIndex] = glm::vec2(300.0f, 300.0f);
	AppData::rotations[currentIndex] = 0.0f;

	AppData::renderables[currentIndex].color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	AppData::colliders[currentIndex].type = Collider::BOX;

	AppData::rigidbodies[currentIndex].velocity = glm::vec2(0.0f, -0.0f);
	AppData::rigidbodies[currentIndex].acceleration = glm::vec2(0.0f, -0.0f);

	AppData::numEntities++;
}

void SceneManager::CreateSceneUI()
{
	ImGui::Begin("Scene Statistics");
	ImGui::Text("Number of Entities: %d/%d", AppData::numEntities, MAX_ENTS);
	if (ImGui::Button("Add Entity"))
	{
		for (int i = 0; i < 500; i++)
		{

		AddEntity((CompTags::TransformTagShortcut |
			CompTags::Collider | CompTags::Renderable | CompTags::Rigidbody));
		}
	}
	ImGui::End();
}
