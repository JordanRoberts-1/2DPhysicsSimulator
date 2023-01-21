#pragma once

#include <vector>
#include <array>
#include <numbers>
#include <algorithm>
#include <iostream>
#include <optional>
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"

#include "ECS.h"

std::array<glm::vec2, 4> GetQuadCornersFromCenterAndRotation(const glm::vec2& center, const glm::vec2& scale, float rotationDegrees)
{
	//from: https://stackoverflow.com/questions/41898990/find-corners-of-a-rotated-rectangle-given-its-center-point-and-rotation
	std::array<glm::vec2, 4> result;
	float width2 = scale.x / 2;
	float height2 = scale.y / 2;
	float angle = -rotationDegrees * (std::numbers::pi / 180.0f);

	//BOTTOM LEFT VERTEX :
	result[0].x = center.x - ((width2) * std::cos(angle)) + ((height2) * std::sin(angle));
	result[0].y = center.y - ((width2) * std::sin(angle)) - ((height2) * std::cos(angle));

	//BOTTOM RIGHT VERTEX :
	result[1].x = center.x + ((width2) * std::cos(angle)) + ((height2) * std::sin(angle));
	result[1].y = center.y + ((width2) * std::sin(angle)) - ((height2) * std::cos(angle));

	//TOP LEFT VERTEX :
	result[2].x = center.x - ((width2) * std::cos(angle)) - ((height2) * std::sin(angle));
	result[2].y = center.y - ((width2) * std::sin(angle)) + ((height2) * std::cos(angle));

	//TOP RIGHT VERTEX :
	result[3].x = center.x + ((width2) * std::cos(angle)) - ((height2) * std::sin(angle));
	result[3].y = center.y + ((width2) * std::sin(angle)) + ((height2) * std::cos(angle));

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
	for (size_t i = 0; i < points.size(); i++)
	{
		float dotProduct = (glm::dot(points[i], axis));
		float axisMagSquared = glm::length2(axis);
		pointsProjected[i].x = (dotProduct * axis.x) / axisMagSquared;
		pointsProjected[i].y = (dotProduct * axis.y) / axisMagSquared;
	}

	return pointsProjected;
}

std::array<float, 4> ScalarProjectPointsOntoAxis(const std::array<glm::vec2, 4>& projectionOntoAxis, const glm::vec2 axis)
{
	std::array<float, 4> scalarProjectionsA;
	for (size_t i = 0; i < projectionOntoAxis.size(); i++)
	{
		scalarProjectionsA[i] = glm::dot(projectionOntoAxis[i], axis);
	}
	return scalarProjectionsA;
}

std::tuple<bool, float, glm::vec2, glm::vec2> BoxBoxCollisionDetection(const std::array<glm::vec2, 4>& A, const std::array<glm::vec2, 4>& B)
{
	std::array<glm::vec2, 4> axisToCheck = CreateAxis(A, B);
	float minOverlap = std::numeric_limits<float>::infinity();
	glm::vec2 minAxis = glm::vec2(std::numeric_limits<float>::infinity());

	int minAxisMaxAIndex = 0, minAxisMinBIndex = 0;
	uint32_t indexMinProjectionAxis = 0;
	for (size_t i = 0; i < axisToCheck.size(); i++)
	{
		std::array<glm::vec2, 4> projectionOntoAxisA = ProjectPointsOntoAxis(axisToCheck[i], A);
		std::array<float, 4> scalarProjectionsA = ScalarProjectPointsOntoAxis(projectionOntoAxisA, axisToCheck[i]);

		std::array<glm::vec2, 4> projectionOntoAxisB = ProjectPointsOntoAxis(axisToCheck[i], B);
		std::array<float, 4> scalarProjectionsB = ScalarProjectPointsOntoAxis(projectionOntoAxisB, axisToCheck[i]);

		const auto& [minA, maxA] = std::minmax_element(scalarProjectionsA.begin(), scalarProjectionsA.end());
		const auto& [minB, maxB] = std::minmax_element(scalarProjectionsB.begin(), scalarProjectionsB.end());

		//These objects do not overlap on this axis, therefore, they CANNOT intersect, so return
		if (*minB > *maxA || *maxB < *minA)
		{
			return { false, minOverlap, minAxis, glm::vec2(0.0f)};
		}

		//They do overlap, so find the overlap and store the index if it's the minimum overlap found already
		float overlap = std::min(*maxA, *maxB) - std::max(*minA, *minB);
		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			minAxis = axisToCheck[i];
			minAxisMaxAIndex = maxA - scalarProjectionsA.begin();
			minAxisMinBIndex = minB - scalarProjectionsB.begin();
			indexMinProjectionAxis = i;
		}
	}

	glm::vec2 contactPoint;
	if (indexMinProjectionAxis == 0 || indexMinProjectionAxis == 1)
	{
		//This axis originated from objectA, so only check the point in object B
		contactPoint = B[minAxisMinBIndex];
	} else if(indexMinProjectionAxis == 2 || indexMinProjectionAxis == 3)
	{
		//This axis originated from object B, so only check the point in object A
		contactPoint = A[minAxisMaxAIndex];
	}

	return { true, minOverlap, minAxis, contactPoint };
}

float Cross(const glm::vec2& vec1, const glm::vec2& vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

glm::vec2 CalculateImpulse(const Rigidbody& rbA, const Rigidbody& rbB, const glm::vec2& rA, const glm::vec2& rB, const glm::vec2& collisionNormal,
	const float& invMomentA, const float& invMomentB)
{
	float restitution = std::min(rbA.restitution, rbB.restitution);
	float rACrossNormal = Cross(rA, collisionNormal);
	float rBCrossNormal = Cross(rB, collisionNormal);
	float coeffRest = -1 - restitution;
	float numerator = coeffRest * glm::dot((rbA.velocity - rbB.velocity), collisionNormal);
	float denomA = ((rACrossNormal) * invMomentA) * rACrossNormal;
	float denomB = ((rBCrossNormal) * invMomentB) * rBCrossNormal;
	float denominator = rbA.invMass + rbB.invMass +
		denomA + denomB;

	float j = numerator / denominator;
	return j * collisionNormal;
}

void ResolveCollision(Entity entityA, Entity entityB, glm::vec2& collisionNormal, float overlap, const glm::vec2& collisionPoint)
{

	Rigidbody& rbA = AppData::rigidbodies[entityA];
	Rigidbody& rbB = AppData::rigidbodies[entityB];

	//If they are both static, then don't resolve the collision
	if (rbA.invMass == 0.0f && rbB.invMass == 0.0f && rbA.invMoment == 0.0f && rbB.invMoment == 0.0f)
		return;

	Position& posA = AppData::positions[entityA];
	Position& posB = AppData::positions[entityB];

	glm::vec2 displacement = posB - posA;
	glm::vec2 relativeVelocity = rbB.velocity - rbA.velocity;

	float movingTowards = glm::dot(relativeVelocity, displacement);
	float velAlongNormal = glm::dot(relativeVelocity, collisionNormal);

	//If they are moving away, then don't do anything
	if (velAlongNormal > 0)
		return;

	//https://fotino.me/2d-rigid-body-collision-response/
	glm::vec2 rA = collisionPoint - posA;
	glm::vec2 rB = collisionPoint - posB;
	const glm::vec2& scaleA = AppData::scales[entityA];
	const glm::vec2& scaleB = AppData::scales[entityB];

	float invMomentA = rbA.invMoment;
	float invMomentB = rbB.invMoment;

	glm::vec2 J = CalculateImpulse(rbA, rbB, rA, rB, collisionNormal, invMomentA, invMomentB);

	//Apply impulse
	rbA.velocity += rbA.invMass * J;
	rbB.velocity -= rbB.invMass * J;
	
	if (!rbA.staticAngular)
	{
		rbA.angularVelocity -= (Cross(rA, J) * invMomentA) * (180.0f / std::numbers::pi);
	}
	if (!rbB.staticAngular)
	{
		rbB.angularVelocity += (Cross(rB, J) * invMomentB) * (180.0f / std::numbers::pi);
	}

	//Positional correction to fix sinking and jittering
	const float percent = 0.1f; // usually 20% to 80% 
	const float slop = 0.02f;// usually 0.01 to 0.1 
	glm::vec2 correction = std::max(overlap - slop, 0.0f) 
		/ (rbA.invMass + rbB.invMass) * percent * collisionNormal;
	AppData::positions[entityA] -= rbA.invMass * correction;
	AppData::positions[entityB] += rbB.invMass * correction;
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

			glm::vec2& posA = AppData::positions[entityA];
			const glm::vec2& scaleA = AppData::scales[entityA];

			for (Entity entityB = entityA + 1; entityB < AppData::numEntities; entityB++)
			{
				if ((AppData::tags[entityB] & rqdTags) != rqdTags)
					continue;


				glm::vec2& posB = AppData::positions[entityB];
				const glm::vec2& scaleB = AppData::scales[entityB];

				//Quick check to see if it's even possible to collide
				if (glm::length2(posB - posA) > glm::length2(scaleA + scaleB) * 2.0f)
				{
					continue;
				}

				Collider& colliderA = AppData::colliders[entityA];
				Collider& colliderB = AppData::colliders[entityB];
				const float& rotA = AppData::rotations[entityA];
				const float& rotB = AppData::rotations[entityB];
				std::array<glm::vec2, 4> cornersA = GetQuadCornersFromCenterAndRotation(posA, scaleA, rotA);
				std::array<glm::vec2, 4> cornersB = GetQuadCornersFromCenterAndRotation(posB, scaleB, rotB);


				if (colliderA.type == Collider::BOX && colliderB.type == Collider::BOX)
				{
					const auto& [bCollision, overlap, collisionNormal, collisionPoint] = BoxBoxCollisionDetection(cornersA, cornersB);

					if (bCollision)
					{
						glm::vec2 displacement = posB - posA;
						glm::vec2 n = collisionNormal;

						//Flip the normal if it doesn't go from A -> B (this is because of the order of collision detecting)
						if (glm::dot(displacement, n) < 0.0f)
						{
							n *= -1.0f;
						}

						ResolveCollision(entityA, entityB, n, overlap, collisionPoint);
					}
				}
			}
		}
	}
}