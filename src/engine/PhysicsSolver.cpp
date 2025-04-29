#include "PhysicsSolver.h"

#include "glm/gtc/matrix_transform.hpp"

PhysicsSolver::PhysicsSolver(const float worldSize, const float circleRadius, const float circleDiameter, const float gravity, const int maxObjects)
    : m_WorldSize(worldSize), m_CircleRadius(circleRadius), m_CircleDiameter(circleDiameter), m_Gravity(gravity),
      m_MaxObjects(maxObjects), m_CollisionGrid((int)m_WorldSize, (int)m_CircleDiameter) {
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

int PhysicsSolver::addObject(const glm::vec2 position) {
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
        m_CollisionGrid.addObject((int)object.position.x, (int)object.position.y, (int)object.getId());
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

void PhysicsSolver::checkAtomCellCollision(const int idx, const int cellIdx) {
    if (cellIdx < 0 || cellIdx >= m_CollisionGrid.size) {
        return;
    }

    const CollisionCell &c = m_CollisionGrid.cells[cellIdx];

    for (int i = 0; i < c.objectCount; i++) {
        solveContact(idx, c.objects[i]);
    }
}

void PhysicsSolver::checkCellCollision(const CollisionCell &c, const int idx) {
    for (int i = 0; i < c.objectCount; i++) {
        const int atomIdx = c.objects[i];

        checkAtomCellCollision(atomIdx, idx - 1);
        checkAtomCellCollision(atomIdx, idx);
        checkAtomCellCollision(atomIdx, idx + 1);
        checkAtomCellCollision(atomIdx, idx + m_CollisionGrid.height - 1);
        checkAtomCellCollision(atomIdx, idx + m_CollisionGrid.height);
        checkAtomCellCollision(atomIdx, idx + m_CollisionGrid.height + 1);
        checkAtomCellCollision(atomIdx, idx - m_CollisionGrid.height - 1);
        checkAtomCellCollision(atomIdx, idx - m_CollisionGrid.height);
        checkAtomCellCollision(atomIdx, idx - m_CollisionGrid.height + 1);
    }
}

void PhysicsSolver::solveCollisions() {
    constexpr int SLICE_COUNT = 2 * THREAD_COUNT;
    const int sliceSize = m_CollisionGrid.width / SLICE_COUNT * m_CollisionGrid.height;
    const int lastCell = SLICE_COUNT * sliceSize;

    for (int i = 0; i < THREAD_COUNT; i++) {
        const int start = 2 * i * sliceSize;
        const int end = start + sliceSize;

        m_Threads[i] = std::thread([start, end, this] {
            for (int j = start; j < end; j++) {
                checkCellCollision(m_CollisionGrid.cells[j], j);
            }
        });
    }

    for (auto &thread : m_Threads) {
        thread.join();
    }

    if (lastCell < m_CollisionGrid.size) {
        for (int j = lastCell; j < m_CollisionGrid.size; j++) {
            checkCellCollision(m_CollisionGrid.cells[j], j);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        const int start = (2 * i + 1) * sliceSize;
        const int end = start + sliceSize;

        m_Threads[i] = std::thread([start, end, this] {
            for (int j = start; j < end; j++) {
                checkCellCollision(m_CollisionGrid.cells[j], j);
            }
        });
    }

    for (auto &thread : m_Threads) {
        thread.join();
    }
}

void PhysicsSolver::updateObjects(const float deltaTime) {
    const int sliceSize = count / THREAD_COUNT;
    std::thread threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        const int start = i * sliceSize;
        const int end = start + sliceSize;

        threads[i] = std::thread([start, end, deltaTime, this] {
            for (int j = start; j < end; j++) {
                auto &object = objects[j];
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
        });
    }

    for (auto &thread : threads) {
        thread.join();
    }
}
