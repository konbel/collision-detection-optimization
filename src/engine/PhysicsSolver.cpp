#include "PhysicsSolver.h"

#include <iostream>
#include <ostream>

#include "glm/gtc/matrix_transform.hpp"

PhysicsSolver::PhysicsSolver(const float worldSize, const float circleRadius, const float circleDiameter, const float gravity, const int maxObjects)
    : m_WorldSize(worldSize), m_CircleRadius(circleRadius), m_CircleDiameter(circleDiameter), m_Gravity(gravity),
      m_MaxObjects(maxObjects) {
    objects.reserve(m_MaxObjects);
}

void PhysicsSolver::update(const float deltaTime) {
    const float subDeltaTime = deltaTime / (float)SUB_STEPS;
    for (int k = 0; k < SUB_STEPS; k++) {
        for (int i = 0; i < count; i++) {
            auto &object1 = objects[i];

            for (int j = 0; j < count; j++) {
                if (i == j) {
                    continue;
                }

                auto &object2 = objects[j];

                glm::vec2 o2_o1 = object1.position - object2.position;
                const float dist2 = o2_o1.x * o2_o1.x + o2_o1.y * o2_o1.y;
                if (dist2 < (m_CircleDiameter) * (m_CircleDiameter) && dist2 > 0.0001) {
                    const float dist = sqrtf(dist2);
                    const float overlap = (m_CircleDiameter - dist) * 0.5f;

                    const glm::vec2 colVec = (o2_o1 / dist) * overlap;

                    object1.position += colVec;
                    object2.position -= colVec;
                }
            }
        }

        updateObjects(subDeltaTime);
    }
}

int PhysicsSolver::addObject(glm::vec2 position) {
    if (count < m_MaxObjects) {
        const GameObject object(count, position);
        objects.push_back(object);
        return count++;
    }

    return -1;
}

void PhysicsSolver::updateObjects(const float deltaTime) {
    for (int i = 0; i < count; i++) {
        auto &object = objects[i];
        object.acceleration += glm::vec2(0.0f, m_Gravity);
        object.update(deltaTime);

        if (object.position.x > m_WorldSize - m_CircleRadius) {
            object.position.x = m_WorldSize - m_CircleRadius;
        } else if (object.position.x < -(m_WorldSize - m_CircleRadius)) {
            object.position.x = -(m_WorldSize - m_CircleRadius);
        }
        if (object.position.y > m_WorldSize - m_CircleRadius) {
            object.position.y = m_WorldSize - m_CircleRadius;
        } else if (object.position.y < -(m_WorldSize - m_CircleRadius)) {
            object.position.y = -(m_WorldSize - m_CircleRadius);
        }
    }
}
