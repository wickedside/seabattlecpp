// Ship.h
#pragma once
#include <utility>

enum class ShipStatus {
    ALIVE,
    HIT,
    SUNK
};

enum class Direction {
    HORIZONTAL,
    VERTICAL
};

class Ship {
private:
    std::pair<int, int> startCoordinate;
    int length;
    Direction direction;
    int hitCount;

public:
    Ship(const std::pair<int, int>& startCoordinate, int len, Direction dir)
        : startCoordinate(startCoordinate), length(len), direction(dir), hitCount(0) {}

    ShipStatus getStatus() const;
    std::pair<int, int> getStartCoordinate() const;
    int getLength() const;
    Direction getDirection() const;

    bool isAt(const std::pair<int, int>& coordinate) const;
    bool hit(const std::pair<int, int>& coordinate);
};
