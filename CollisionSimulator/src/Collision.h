#include <vector>
#include <array>
#include <numbers>
#include <algorithm>
#include "glm/glm.hpp"
#include "ECS.h"
#include <iostream>

std::array<glm::vec2, 4> GetQuadCornersFromCenterAndRotation(glm::vec2 center, glm::vec2 scale, float rotationDegrees)
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
	result[0].x = A[3].x - A[2].x;
	result[0].y = A[3].y - A[2].y;
	result[1].x = A[3].x - A[1].x;
	result[1].y = A[3].y - A[1].y;

	result[2].x = B[2].x - B[0].x;
	result[2].y = B[2].y - B[0].y;
	result[3].x = B[2].x - B[3].x;
	result[3].y = B[2].y - B[3].y;
	return result;
}

std::array<glm::vec2, 4> ProjectPointsOntoAxis(glm::vec2 axis, const std::array<glm::vec2, 4>& points)
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

bool BoxBoxCollisionDetection(const std::array<glm::vec2, 4>& A, const std::array<glm::vec2, 4>& B)
{
	std::array<glm::vec2, 4> axisToCheck = CreateAxis(A, B);
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
		if (*minB > *maxA || *maxB < *minA)
		{
			return false;
		}
	}
	return true;
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

			const glm::vec2& posA = AppData::positions[entityA];
			const glm::vec2& scaleA = AppData::scales[entityA];
			const float& rotA = AppData::rotations[entityA];
			std::array<glm::vec2, 4> cornersA = GetQuadCornersFromCenterAndRotation(posA, scaleA, rotA);

			for (Entity entityB = entityA + 1; entityB < AppData::numEntities; entityB++)
			{
				if ((AppData::tags[entityB] & rqdTags) != rqdTags)
					continue;

				Collider& colliderB = AppData::colliders[entityB];

				const glm::vec2& posB = AppData::positions[entityB];
				const glm::vec2& scaleB = AppData::scales[entityB];
				const float& rotB = AppData::rotations[entityB];
				std::array<glm::vec2, 4> cornersB = GetQuadCornersFromCenterAndRotation(posB, scaleB, rotB);

				if (colliderA.type == Collider::BOX && colliderB.type == Collider::BOX)
				{
					bool collision = BoxBoxCollisionDetection(cornersA, cornersB);
					if (collision)
					{
						std::cout << "Collision occurred! " << std::endl;
					}
				}
			}
		}
	}
}