#pragma once

#include "Board.h"
#include <string>

class Player {
protected:
    Board ownBoard;
    Board opponentBoard;
    void placeShip(int length, int count, const std::string& name);
    bool canPlaceShip(int x, int y, int length, char direction);

public:
    virtual ~Player() {}

    Board& getOwnBoard() { return ownBoard; }
    const Board& getOpponentBoard() const { return opponentBoard; }

    virtual std::pair<int, int> chooseShootCoordinate() = 0; // Выбор координаты для выстрела
    virtual void placeShips() = 0; // Размещение кораблей игрока на доске
    virtual std::string getPlayerType() const = 0; // Возвращает тип игрока ("Человек" или "Компьютер")

    ShipStatus shootAtPlayer(Player& opponent, const std::pair<int, int>& coordinate);
    bool allShipsSunk() const;
};
