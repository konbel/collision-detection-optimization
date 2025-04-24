#ifndef COLLISIONGRID_H
#define COLLISIONGRID_H

#include <vector>

struct CollisionCell {
    static constexpr int CELL_CAPACITY = 4;
    static constexpr int MAX_CELL_IDX = CELL_CAPACITY - 1;

    int objectCount = 0;
    int objects[CELL_CAPACITY] = {};

    CollisionCell() = default;

    void clear() {
        objectCount = 0;
    }

    void addObject(const int object) {
        objects[objectCount] = object;
        objectCount += objectCount < MAX_CELL_IDX;
    }
};

class CollisionGrid {
    int m_Scale;

public:
    int size;
    int width;
    int height;
    std::vector<CollisionCell> cells;

    CollisionGrid(int size, int scale);

    void clear();
    void addObject(int x, int y, int id);
};

#endif //COLLISIONGRID_H
