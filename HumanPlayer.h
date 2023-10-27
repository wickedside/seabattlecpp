#pragma once

#include "Player.h"
#include <iostream>
#include <string>

class HumanPlayer : public Player {
public:
    HumanPlayer();
    ~HumanPlayer();

    std::pair<int, int> chooseShootCoordinate() override; // Выбор координаты для выстрела
    void placeShips() override; // Размещение кораблей игрока на доске
    void autoPlaceShips();
    std::string getPlayerType() const override { return "Человек"; } // Возвращает тип игрока
private:
    Board ownBoard; // Предполагая, что у вас есть класс Board.
};