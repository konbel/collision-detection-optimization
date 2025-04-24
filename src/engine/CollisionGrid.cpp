#include "CollisionGrid.h"

CollisionGrid::CollisionGrid(const int size, const int scale) : m_Scale(scale) {
    width = size / scale + 1;
    height = size / scale + 1;
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
    CollisionCell &cell = cells[x / m_Scale * height + y / m_Scale];
    cell.addObject(id);
}
