#pragma once
#include <cstdint>
#include <vector>
#include <array>

#include "glm/glm.hpp"

using Entity = uint32_t;
using Position = glm::vec2;
using Scale = glm::vec2;
using Rotation = float;
using Velocity = glm::vec2;
using Acceleration = glm::vec2;
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
	constexpr Tag Scale = 0x1 << 1;
	constexpr Tag Rotation = 0x1 << 2;
	constexpr Tag Velocity = 0x1 << 3;
	constexpr Tag Acceleration = 0x1 << 4;
	constexpr Tag Collider = 0x1 << 5;
	constexpr Tag Renderable = 0x1 << 6;
}

constexpr int MAX_ENTS = 1000;

namespace AppData
{
	inline std::array<Tag, MAX_ENTS> tags;
	inline uint32_t numEntities = 0;
	
	inline std::array<Position, MAX_ENTS> positions;
	inline std::array<Scale, MAX_ENTS> scales;
	inline std::array<Rotation, MAX_ENTS> rotations;
	inline std::array<Velocity, MAX_ENTS> velocities;
	inline std::array<Acceleration, MAX_ENTS> accelerations;
	inline std::array<Collider, MAX_ENTS> colliders;
	inline std::array<Renderable, MAX_ENTS> renderables;
}

namespace Systems{
	void ProcessKinematics();
}

