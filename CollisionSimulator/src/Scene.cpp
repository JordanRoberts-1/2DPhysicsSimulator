#include "Scene.h"

#include <iostream>
#include <ImGUI/imgui.h>

#include "ECS.h"
#include "Util/Input.h"

void SceneManager::CreateScene()
{
	glm::vec2 floorScales(3840.0f, 1500.0f);
	float floorRotation = 30.0f;

	int floorL = AddSquare();
	if (floorL == -1)
		return;

	AppData::positions[floorL] = glm::vec2(-960.0f, -1080.0f);
	AppData::scales[floorL] = floorScales;
	Systems::SetRotation(floorL, floorRotation);
	AppData::renderables[floorL].color = glm::vec4(0.0f, 0.7f, 0.2f, 1.0f);
	
	Rigidbody& floorLRb = AppData::rigidbodies[floorL];
	floorLRb.velocity = glm::vec2(0.0f);
	floorLRb.acceleration = glm::vec2(0.0f);
	floorLRb.invMass = 0.0f;
	floorLRb.invMoment = 0.0f;
	floorLRb.staticAngular = true;

	int floorR = AddSquare();
	if (floorR == -1)
		return;

	AppData::positions[floorR] = glm::vec2(960.0f, -1080.0f);
	AppData::scales[floorR] = floorScales;
	Systems::SetRotation(floorR, -floorRotation);
	AppData::renderables[floorR].color = glm::vec4(0.0f, 0.7f, 0.2f, 1.0f);

	Rigidbody& floorRRb = AppData::rigidbodies[floorR];
	floorRRb.velocity = glm::vec2(0.0f);
	floorRRb.acceleration = glm::vec2(0.0f);
	floorRRb.invMass = 0.0f;
	floorRRb.invMoment = 0.0f;
	floorRRb.staticAngular = true;

	glm::vec2 platScales(700.0f, 50.0f);
	float platRot = 15.0f;

	int platL = AddSquare();
	if (platL == -1)
		return;

	AppData::positions[platL] = glm::vec2(-330.0f, 600.0f);
	AppData::scales[platL] = platScales;
	Systems::SetRotation(platL, -platRot);
	AppData::renderables[platL].color = glm::vec4(0.4f, 0.2f, 0.0f, 1.0f);

	Rigidbody& platLRb = AppData::rigidbodies[platL];
	platLRb.velocity = glm::vec2(0.0f);
	platLRb.acceleration = glm::vec2(0.0f);
	platLRb.invMass = 0.0f;
	platLRb.invMoment = 0.0f;
	platLRb.staticAngular = true;

	int platR = AddSquare();
	if (platR == -1)
		return;

	AppData::positions[platR] = glm::vec2(330.0f, 600.0f);
	AppData::scales[platR] = platScales;
	Systems::SetRotation(platR, platRot);
	AppData::renderables[platR].color = glm::vec4(0.4f, 0.2f, 0.0f, 1.0f);

	Rigidbody& platRRb = AppData::rigidbodies[platR];
	platRRb.velocity = glm::vec2(0.0f);
	platRRb.acceleration = glm::vec2(0.0f);
	platRRb.invMass = 0.0f;
	platRRb.invMoment = 0.0f;
	platRRb.staticAngular = true;


	float wallWidth = 50.0f;

	int wallL = AddSquare();
	if (wallL == -1)
		return;

	AppData::positions[wallL] = glm::vec2(-1920.0f - wallWidth / 2.0f, 0.0f);
	AppData::scales[wallL] = glm::vec2(wallWidth, 2160.0f);
	Systems::SetRotation(wallL, 0.0f);
	AppData::renderables[wallL].color = glm::vec4(1.0f);

	Rigidbody& wallLRb = AppData::rigidbodies[wallL];
	wallLRb.velocity = glm::vec2(0.0f);
	wallLRb.acceleration = glm::vec2(0.0f);
	wallLRb.invMass = 0.0f;
	wallLRb.invMoment = 0.0f;
	wallLRb.staticAngular = true;

	int wallR = AddSquare();
	if (wallR == -1)
		return;

	AppData::positions[wallR] = glm::vec2(1920.0f + wallWidth / 2.0f, 0.0f);
	AppData::scales[wallR] = glm::vec2(wallWidth, 2160.0f);
	Systems::SetRotation(wallR, 0.0f);
	AppData::renderables[wallR].color = glm::vec4(1.0f);

	Rigidbody& wallRRb = AppData::rigidbodies[wallR];
	wallRRb.velocity = glm::vec2(0.0f);
	wallRRb.acceleration = glm::vec2(0.0f);
	wallRRb.invMass = 0.0f;
	wallRRb.invMoment = 0.0f;
	wallRRb.staticAngular = true;

	int wallT = AddSquare();
	if (wallT == -1)
		return;

	AppData::positions[wallT] = glm::vec2(0.0f, 1080.0f + wallWidth / 2.0f);
	AppData::scales[wallT] = glm::vec2(3840.0f, wallWidth);
	Systems::SetRotation(wallT, 0.0f);
	AppData::renderables[wallT].color = glm::vec4(1.0f);

	Rigidbody& wallTRb = AppData::rigidbodies[wallT];
	wallTRb.velocity = glm::vec2(0.0f);
	wallTRb.acceleration = glm::vec2(0.0f);
	wallTRb.invMass = 0.0f;
	wallTRb.invMoment = 0.0f;
	wallTRb.staticAngular = true;
}

int SceneManager::AddSquare()
{
	uint32_t currentIndex = AddEntity(CompTags::TransformTagShortcut |
		CompTags::Collider | CompTags::Renderable | CompTags::Rigidbody);

	if (currentIndex == -1)
	{
		return -1;
	}

	AppData::positions[currentIndex] = glm::vec2(0.0f, 800.0f);
	AppData::scales[currentIndex] = glm::vec2(50.0f);
	Systems::SetRotation(currentIndex, 0.0f + 10.0f * currentIndex);

	AppData::colliders[currentIndex].type = Collider::BOX;

	AppData::renderables[currentIndex].color = glm::vec4(1.0f);

	Rigidbody& rb = AppData::rigidbodies[currentIndex];
	rb.velocity = glm::vec2(-0.0f, 10.0f);
	rb.acceleration = glm::vec2(0.0f, -0.1f);
	rb.angularVelocity = 0.0f;
	rb.invMass = 1.0f / (glm::length(AppData::scales[currentIndex]) * 17.0f);
	rb.restitution = 0.4f;
	rb.staticAngular = false;

	Scale& scale = AppData::scales[currentIndex];
	rb.invMoment = 12.0f / (std::pow(scale.x, 3) * scale.y);

	return currentIndex;
}

int SceneManager::AddEntity(Tag tag)
{
	if(AppData::numEntities >= MAX_ENTS)
	{
		return -1;
	}

	uint32_t currentIndex = AppData::numEntities;
	AppData::tags[currentIndex] = tag;
	AppData::numEntities++;
	return currentIndex;
}

void SceneManager::CreateSceneUI()
{
	ImGui::Begin("Scene Statistics");
	ImGui::Text("Number of Entities: %d/%d", AppData::numEntities, MAX_ENTS);
	if (ImGui::Button("Add 200 entities"))
	{
		for (int i = 0; i < 200; i++)
		{
			AddSquare();
		}
	}
	ImGui::End();
}

void SceneManager::SceneUpdate()
{
	if (Input::s_MouseHeld)
	{
		int square = AddSquare();
		if (square == -1)
			return;

		AppData::positions[square] = glm::vec2(AppData::cursorX, AppData::cursorY);
	}
}
