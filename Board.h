#pragma once

#include <vector>
#include <utility>
#include "Ship.h"

class Board {
private:
    static const int BOARD_SIZE = 10;
    char grid[BOARD_SIZE][BOARD_SIZE];
    std::vector<Ship> ships;

    //bool isValidPlacement(const Ship& ship) const;

public:
    Board();

    static int getBoardSize() { return BOARD_SIZE; }

    bool addShip(const Ship& ship);
    ShipStatus shootAt(const std::pair<int, int>& coordinate);
    const std::vector<Ship>& getShips() const;
    char getCell(const std::pair<int, int>& coordinate) const;
    void display() const;
    void displayEnemyBoard() const;
    bool canPlaceShip(int x, int y, char direction, int size) const;
    void placeShip(int x, int y, char direction, int size);
    void markSurroundingAsMiss(const Ship& ship);
    bool isCellShot(int x, int y) const;
    void setCell(const std::pair<int, int>& coordinate, char value);
};