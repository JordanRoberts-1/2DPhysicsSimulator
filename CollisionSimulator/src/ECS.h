#pragma once
#include <cstdint>
#include <vector>
#include <array>

#include "glm/glm.hpp"


constexpr int MAX_ENTS = 500;
const float DRAG = 0.01f;
const float ROTATIONAL_DRAG = 0.1f;

using Entity = uint32_t;
using Position = glm::vec2;
using Scale = glm::vec2;
using Rotation = float;
struct Collider
{
	enum Type {BOX, CIRCLE};
	Type type;
	float radius;
};
struct Renderable
{
	glm::vec4 color;
};

struct Rigidbody
{
	glm::vec2 velocity;
	glm::vec2 acceleration;
	float angularVelocity;
	float invMass;
	float restitution;
	bool staticAngular;
	float invMoment;
};


using Tag = uint32_t;
namespace CompTags
{
	constexpr Tag Position = 0x1;
	constexpr Tag Scale = 0x1 << 1;
	constexpr Tag Rotation = 0x1 << 2;
	constexpr Tag Collider = 0x1 << 3;
	constexpr Tag Renderable = 0x1 << 4;
	constexpr Tag Rigidbody = 0x1 << 5;
	constexpr Tag TransformTagShortcut = CompTags::Position | CompTags::Scale | CompTags::Rotation;
}

namespace AppData
{
	inline std::array<Tag, MAX_ENTS> tags;
	inline uint32_t numEntities = 0;
	
	inline std::array<Position, MAX_ENTS> positions;
	inline std::array<Scale, MAX_ENTS> scales;
	inline std::array<Rotation, MAX_ENTS> rotations;
	inline std::array<Collider, MAX_ENTS> colliders;
	inline std::array<Renderable, MAX_ENTS> renderables;
	inline std::array<Rigidbody, MAX_ENTS> rigidbodies;
}

namespace Systems{
	void SetRotation(Entity e, float rotationDegrees);

	void ProcessKinematics();
	void ProcessCollisionDetection();
}

