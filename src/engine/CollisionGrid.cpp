#include "CollisionGrid.h"

CollisionGrid::CollisionGrid(const int size) {
    width = size / 2 + 1;
    height = size / 2 + 1;
    this->size = width * height;

    cells.reserve(this->size);
    for (int i = 0; i < this->size; i++) {
        cells.emplace_back(CollisionCell());
    }
}

void CollisionGrid::clear() {
    for (auto &cell : cells) {
        cell.clear();
    }
}

void CollisionGrid::addObject(const int x, const int y, const int id) {
    CollisionCell &cell = cells[x / 2 * height + y / 2];
    cell.addObject(id);
}
