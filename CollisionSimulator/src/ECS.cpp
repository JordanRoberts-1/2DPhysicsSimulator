#include "ECS.h"

namespace Systems
{
	void ProcessKinematics()
	{
		constexpr Tag rqdTags = CompTags::Position | CompTags::Velocity;
		for (Entity e = 0; e < AppData::numEntities; e++)
		{
			if ((AppData::tags[e] & rqdTags) != rqdTags) { continue; }
			Acceleration& a = AppData::accelerations[e];
			Velocity& v = AppData::velocities[e];
			Position& p = AppData::positions[(int)e];

			v += a;
			p += v;
		}
	}
}