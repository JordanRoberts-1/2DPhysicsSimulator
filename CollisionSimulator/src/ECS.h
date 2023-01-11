#pragma once
#include <cstdint>
#include <vector>
#include <array>

#include "glm/glm.hpp"

using Entity = uint32_t;
using Position = glm::vec3;
using Velocity = glm::vec3;
struct Collider
{
	float radius;
};
struct Renderable
{
	glm::vec4 color;
};

using Tag = uint32_t;
namespace CompTags
{
	constexpr Tag Position = 0x1;
	constexpr Tag Velocity = 0x1 << 1;
	constexpr Tag Collider = 0x1 << 2;
	constexpr Tag Renderable = 0x1 << 3;
}

constexpr int MAX_ENTS = 1000;

namespace AppData
{
	inline std::array<Tag, MAX_ENTS> tags;
	
	inline std::array<Position, MAX_ENTS> positions;
	inline std::array<Velocity, MAX_ENTS> velocities;
	inline std::array<Collider, MAX_ENTS> colliders;
	inline std::array<Renderable, MAX_ENTS> renderables;
}

//namespace Systems{
//	void processPhysics(float dt)
//	{
//		constexpr Tag rqdTags = CompTags::Position | CompTags::Velocity;
//		for (Entity e = 0; e < MAX_ENTS; e++)
//		{
//			if ((AppData::tags[e] & rqdTags) != rqdTags) { continue; }
//			Velocity& v = AppData::velocities[e];
//			Position& p = AppData::positions[(int)e];
//		}
//	}
//}

