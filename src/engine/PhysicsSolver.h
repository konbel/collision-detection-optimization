#ifndef PHYSICSSOLVER_H
#define PHYSICSSOLVER_H

#include <thread>

#include "GameObject.h"
#include "CollisionGrid.h"

class PhysicsSolver {
    constexpr static int SUB_STEPS = 8;
    static constexpr int THREAD_COUNT = 10;

    float m_WorldSize;
    float m_CircleRadius;
    float m_CircleDiameter;
    float m_Gravity;
    int m_MaxObjects;
    CollisionGrid m_CollisionGrid;
    std::thread m_Threads[THREAD_COUNT];

    void addObjectsToGrid();
    void solveContact(int atomIdx1, int atomIdx2);
    void checkAtomCellCollision(int idx, int cellIdx);
    void checkCellCollision(const CollisionCell &c, int idx);
    void solveCollisions();
    void updateObjects(float deltaTime);

public:
    std::vector<GameObject> objects;
    int count = 0;

    PhysicsSolver(float worldSize, float circleRadius, float circleDiameter, float gravity, int maxObjects);
    ~PhysicsSolver() = default;

    void update(float deltaTime);
    int addObject(glm::vec2 position);
};

#endif //PHYSICSSOLVER_H
