#include "ECS.h"

namespace Systems
{
	void ProcessKinematics()
	{
		constexpr Tag rqdTags = CompTags::Position | CompTags::Rigidbody;
		for (Entity e = 0; e < AppData::numEntities; e++)
		{
			if ((AppData::tags[e] & rqdTags) != rqdTags) { continue; }
			Rigidbody& rb = AppData::rigidbodies[e];
			glm::vec2& acceleration = rb.acceleration;
			glm::vec2& velocity = rb.velocity;
			Position& p = AppData::positions[(int)e];

			velocity += acceleration;
			p += velocity;
		}
	}
}