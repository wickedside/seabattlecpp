#pragma once

#include "Player.h"
#include <cstdlib> // для rand()
#include <vector>

class ComputerPlayer : public Player {
public:
    ComputerPlayer();
    ~ComputerPlayer();

    std::pair<int, int> chooseShootCoordinate() override;
    void placeShips() override;
    std::string getPlayerType() const override { return "Компьютер"; }

    enum class Mode { RANDOM, INTELLIGENT };
    void setShootingMode(Mode mode);
private:
    Mode shootingMode = Mode::RANDOM;
    std::vector<std::pair<int, int>> intelligentTargets; // возможные цели в интеллектуальном режиме

    void generateRandomShips();
    bool canPlaceShip(const int x, const int y, const char direction, const int size);

    std::pair<int, int> lastHit; // последнее попадание
    bool shipDirectionDetermined = false; // направление корабля определено
    char shipDirection; // 'h' или 'v'

    void addSurroundingCoordinates(int x, int y);
    void resetShootingStrategy();
};
