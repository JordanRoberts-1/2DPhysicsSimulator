#include "ECS.h"

namespace Systems
{
	//Helper system to set rotation as they need to wrap
	void SetRotation(Entity e, float rotationDegrees)
	{
		Rotation& rotation = AppData::rotations[e];
		rotation = std::fmod(rotationDegrees + 180, 360);
		if (rotation < 0)
			rotation += 360;
		rotation -= 180;
	}

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
			
			Rotation& rot = AppData::rotations[e];
			SetRotation(e, (rot + rb.angularVelocity) * ROTATIONAL_DRAG);
		}
	}
}