// Ship.cpp
#include "Ship.h"

ShipStatus Ship::getStatus() const {
    if (hitCount == length) {
        return ShipStatus::SUNK;
    }
    if (hitCount > 0) {
        return ShipStatus::HIT;
    }
    return ShipStatus::ALIVE;
}

std::pair<int, int> Ship::getStartCoordinate() const {
    return startCoordinate;
}

int Ship::getLength() const {
    return length;
}

Direction Ship::getDirection() const {
    return direction;
}

bool Ship::isAt(const std::pair<int, int>& coordinate) const {
    if (direction == Direction::HORIZONTAL) {
        return coordinate.second == startCoordinate.second &&
            coordinate.first >= startCoordinate.first &&
            coordinate.first < startCoordinate.first + length;
    }
    else {
        return coordinate.first == startCoordinate.first &&
            coordinate.second >= startCoordinate.second &&
            coordinate.second < startCoordinate.second + length;
    }
}

bool Ship::hit(const std::pair<int, int>& coordinate) {
    if (isAt(coordinate)) {
        hitCount++;
        return true;
    }
    return false;
}
