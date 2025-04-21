#include "PhysicsSolver.h"

#include "glm/gtc/matrix_transform.hpp"

PhysicsSolver::PhysicsSolver(const float worldSize, const float circleRadius, const float circleDiameter, const float gravity, const int maxObjects)
    : m_WorldSize(worldSize), m_CircleRadius(circleRadius), m_CircleDiameter(circleDiameter), m_Gravity(gravity),
      m_MaxObjects(maxObjects), m_CollisionGrid(m_WorldSize) {
    objects.reserve(m_MaxObjects);
}

void PhysicsSolver::update(const float deltaTime) {
    const float subDeltaTime = deltaTime / (float)SUB_STEPS;
    for (int k = 0; k < SUB_STEPS; k++) {
        addObjectsToGrid();
        solveCollisions();
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

void PhysicsSolver::addObjectsToGrid() {
    m_CollisionGrid.clear();
    for (const auto &object : objects) {
        m_CollisionGrid.addObject((int)object.position.x, (int)object.position.y, object.getId());
    }
}

void PhysicsSolver::solveContact(const int atomIdx1, const int atomIdx2) {
    if (atomIdx1 == atomIdx2) {
        return;
    }

    GameObject &object1 = objects[atomIdx1];
    GameObject &object2 = objects[atomIdx2];

    const glm::vec2 o2_o1 = object1.position - object2.position;
    const float dist2 = o2_o1.x * o2_o1.x + o2_o1.y * o2_o1.y;
    if (dist2 < m_CircleDiameter * m_CircleDiameter && dist2 > 0.0001) {
        const float dist = sqrtf(dist2);
        const float overlap = (m_CircleDiameter - dist) * 0.5f;

        const glm::vec2 colVec = o2_o1 / dist * overlap;

        object1.position += colVec;
        object2.position -= colVec;
    }
}

void PhysicsSolver::checkAtomCellCollision(const int idx, const CollisionCell &c) {
    for (int i = 0; i < c.objectCount; i++) {
        solveContact(idx, c.objects[i]);
    }
}

void PhysicsSolver::checkCellCollision(const CollisionCell &c, const int idx) {
    for (int i = 0; i < c.objectCount; i++) {
        const int atomIdx = c.objects[i];
        checkAtomCellCollision(atomIdx, m_CollisionGrid.cells[idx - 1]);
        checkAtomCellCollision(atomIdx, m_CollisionGrid.cells[idx]);
        checkAtomCellCollision(atomIdx, m_CollisionGrid.cells[idx + 1]);
        checkAtomCellCollision(atomIdx, m_CollisionGrid.cells[idx + m_CollisionGrid.height - 1]);
        checkAtomCellCollision(atomIdx, m_CollisionGrid.cells[idx + m_CollisionGrid.height]);
        checkAtomCellCollision(atomIdx, m_CollisionGrid.cells[idx + m_CollisionGrid.height + 1]);
        checkAtomCellCollision(atomIdx, m_CollisionGrid.cells[idx - m_CollisionGrid.height - 1]);
        checkAtomCellCollision(atomIdx, m_CollisionGrid.cells[idx - m_CollisionGrid.height]);
        checkAtomCellCollision(atomIdx, m_CollisionGrid.cells[idx - m_CollisionGrid.height + 1]);
    }
}

void PhysicsSolver::solveCollisions() {
    for (int i = 0; i < m_CollisionGrid.size; i++) {
        // skip first and last col
        // if (i < m_CollisionGrid.height || i >= m_CollisionGrid.size - m_CollisionGrid.height) {
        //     continue;
        // }

        // skip first and last row
        // if (i % m_CollisionGrid.height == 0 || i % (m_CollisionGrid.height - 1) == 0) {
        //     continue;
        // }

        checkCellCollision(m_CollisionGrid.cells[i], i);
    }
}

void PhysicsSolver::updateObjects(const float deltaTime) {
    for (int i = 0; i < count; i++) {
        auto &object = objects[i];
        object.acceleration += glm::vec2(0.0f, m_Gravity);
        object.update(deltaTime);

        const float margin = m_CircleRadius;
        if (object.position.x > m_WorldSize - margin) {
            object.position.x = m_WorldSize - margin;
        } else if (object.position.x < margin) {
            object.position.x = margin;
        }
        if (object.position.y > m_WorldSize - margin) {
            object.position.y = m_WorldSize - margin;
        } else if (object.position.y < margin) {
            object.position.y = margin;
        }
    }
}
