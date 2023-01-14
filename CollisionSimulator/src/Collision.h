#include <vector>
#include <array>
#include <numbers>
#include <algorithm>
#include "glm/glm.hpp"
#include "ECS.h"
#include <iostream>

std::array<glm::vec2, 4> GetQuadCornersFromCenterAndRotation(const glm::vec2& center, const glm::vec2& scale, float rotationDegrees)
{
	//from: https://stackoverflow.com/questions/41898990/find-corners-of-a-rotated-rectangle-given-its-center-point-and-rotation
	std::array<glm::vec2, 4> result;
	float width = scale.x, height = scale.y;
	float angle = -rotationDegrees * (std::numbers::pi / 180.0f);

	//BOTTOM LEFT VERTEX :
	result[0].x = center.x - ((width / 2) * std::cos(angle)) + ((height / 2) * std::sin(angle));
	result[0].y = center.y - ((width / 2) * std::sin(angle)) - ((height / 2) * std::cos(angle));

	//BOTTOM RIGHT VERTEX :
	result[1].x = center.x + ((width / 2) * std::cos(angle)) + ((height / 2) * std::sin(angle));
	result[1].y = center.y + ((width / 2) * std::sin(angle)) - ((height / 2) * std::cos(angle));

	//TOP LEFT VERTEX :
	result[2].x = center.x - ((width / 2) * std::cos(angle)) - ((height / 2) * std::sin(angle));
	result[2].y = center.y - ((width / 2) * std::sin(angle)) + ((height / 2) * std::cos(angle));

	//TOP RIGHT VERTEX :
	result[3].x = center.x + ((width / 2) * std::cos(angle)) - ((height / 2) * std::sin(angle));
	result[3].y = center.y + ((width / 2) * std::sin(angle)) + ((height / 2) * std::cos(angle));

	return result;
}

std::array<glm::vec2, 4> CreateAxis(const std::array<glm::vec2, 4>& A, const std::array<glm::vec2, 4>& B)
{
	std::array<glm::vec2, 4> result;
	result[0] = glm::normalize(A[3] - A[2]);
	result[1] = glm::normalize(A[3] - A[1]);

	result[2] = glm::normalize(B[2] - B[0]);
	result[3] = glm::normalize(B[2] - B[3]);
	return result;
}

std::array<glm::vec2, 4> ProjectPointsOntoAxis(const glm::vec2& axis, const std::array<glm::vec2, 4>& points)
{
	std::array<glm::vec2, 4> pointsProjected{};
	for (uint32_t i = 0; i < points.size(); i++)
	{
		float dotProduct = (glm::dot(points[i], axis));
		float axisMag = glm::length(axis);
		float axisMagSquared = axisMag * axisMag;
		pointsProjected[i].x = (dotProduct * axis.x) / axisMagSquared;
		pointsProjected[i].y = (dotProduct * axis.y) / axisMagSquared;
	}

	return pointsProjected;
}

std::tuple<bool, float, glm::vec2> BoxBoxCollisionDetection(const std::array<glm::vec2, 4>& A, const std::array<glm::vec2, 4>& B)
{
	std::array<glm::vec2, 4> axisToCheck = CreateAxis(A, B);
	float minOverlap = std::numeric_limits<float>::infinity();
	glm::vec2 minAxis = glm::vec2(std::numeric_limits<float>::infinity());
	for (uint32_t i = 0; i < axisToCheck.size(); i++)
	{
		std::array<glm::vec2, 4> projectionOntoAxisA = ProjectPointsOntoAxis(axisToCheck[i], A);
		std::array<float, 4> scalarProjectionsA;
		for (uint32_t j = 0; j < projectionOntoAxisA.size(); j++)
		{
			scalarProjectionsA[j] = glm::dot(projectionOntoAxisA[j], axisToCheck[i]);
		}

		std::array<glm::vec2, 4> projectionOntoAxisB = ProjectPointsOntoAxis(axisToCheck[i], B);
		std::array<float, 4> scalarProjectionsB;
		for (uint32_t j = 0; j < projectionOntoAxisB.size(); j++)
		{
			scalarProjectionsB[j] = glm::dot(projectionOntoAxisB[j], axisToCheck[i]);
		}

		const auto& [minA, maxA] = std::minmax_element(scalarProjectionsA.begin(), scalarProjectionsA.end());
		const auto& [minB, maxB] = std::minmax_element(scalarProjectionsB.begin(), scalarProjectionsB.end());

		//These objects do not overlap on this axis, therefore, they CANNOT intersect, so return
		if (*minB > *maxA || *maxB < *minA)
		{
			return { false, minOverlap, minAxis };
		}

		float overlap = std::min(*maxA, *maxB) - std::max(*minA, *minB);
		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			minAxis = axisToCheck[i];
		}
	}
	return { true, minOverlap, minAxis };
}

void ResolveCollision(Entity entityA, Entity entityB, const glm::vec2& collisionNormal, float overlap)
{
	Rigidbody& rbA = AppData::rigidbodies[entityA];
	Rigidbody& rbB = AppData::rigidbodies[entityB];
	Position& posA = AppData::positions[entityA];
	Position& posB = AppData::positions[entityB];


	float restitution = std::min(rbA.restitution, rbB.restitution);

	glm::vec2 relativeVelocity = rbB.velocity - rbA.velocity;
	glm::vec2 displacement = posB - posA;
	float movingTowards = glm::dot(relativeVelocity, displacement);
	float velAlongNormal = glm::dot(relativeVelocity, collisionNormal);

	if (movingTowards > 0)
		return;

	float impulseMag = -(1 + restitution) * velAlongNormal;
	impulseMag /= rbA.invMass + rbB.invMass;

	glm::vec2 impulse = impulseMag * collisionNormal;
	rbA.velocity -= rbA.invMass * impulse;
	rbB.velocity += rbB.invMass * impulse;

	const float percent = 0.2; // usually 20% to 80% 
	const float slop = 0.01;// usually 0.01 to 0.1 
	glm::vec2 correction = std::max(overlap - slop, 0.0f) 
		/ (rbA.invMass + rbB.invMass) * percent * collisionNormal;
	AppData::positions[entityA] -= rbA.invMass * correction;
	AppData::positions[entityB] -= rbB.invMass * correction;
}

namespace Systems
{
	void ProcessCollisionDetection()
	{
		constexpr Tag rqdTags = CompTags::TransformTagShortcut |
			CompTags::Collider | CompTags::Rigidbody;

		for (Entity entityA = 0; entityA < AppData::numEntities; entityA++)
		{
			if ((AppData::tags[entityA] & rqdTags) != rqdTags)
				continue;

			Collider& colliderA = AppData::colliders[entityA];

			glm::vec2& posA = AppData::positions[entityA];
			const glm::vec2& scaleA = AppData::scales[entityA];
			const float& rotA = AppData::rotations[entityA];
			std::array<glm::vec2, 4> cornersA = GetQuadCornersFromCenterAndRotation(posA, scaleA, rotA);

			for (Entity entityB = entityA + 1; entityB < AppData::numEntities; entityB++)
			{
				if ((AppData::tags[entityB] & rqdTags) != rqdTags)
					continue;

				Collider& colliderB = AppData::colliders[entityB];

				glm::vec2& posB = AppData::positions[entityB];
				const glm::vec2& scaleB = AppData::scales[entityB];
				const float& rotB = AppData::rotations[entityB];
				std::array<glm::vec2, 4> cornersB = GetQuadCornersFromCenterAndRotation(posB, scaleB, rotB);

				if (colliderA.type == Collider::BOX && colliderB.type == Collider::BOX)
				{
					const auto& [bCollision, overlap, collisionNormal] = BoxBoxCollisionDetection(cornersA, cornersB);
					if (bCollision)
					{
						ResolveCollision(entityA, entityB, collisionNormal, overlap);
					}
				}
			}
		}
	}
}