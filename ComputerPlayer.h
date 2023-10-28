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
    std::pair<int, int> randomShoot();
    std::pair<int, int> shootAroundInitialHit(); // Правильное имя функции
    std::pair<int, int> shootInDirection();
    void registerHit(int x, int y);
    void registerMiss();
    void resetInitialHit(); // Добавляем объявление функции
    std::pair<int, int> getLastHit() const;

private:
    Mode shootingMode = Mode::RANDOM;
    std::vector<std::pair<int, int>> intelligentTargets; // возможные цели в интеллектуальном режиме

    void autoPlaceShips();
    bool canPlaceShip(const int x, const int y, const char direction, const int size);

    bool shipDirectionDetermined = false; // направление корабля определено
    char shipDirection; // 'h' или 'v'

    void addSurroundingCoordinates(int x, int y);
    void resetShootingStrategy();
    std::pair<int, int> lastHit = { -1, -1 };
    std::pair<int, int> initialHit = { -1, -1 }; // Добавляем объявление переменной
    char lastHitDirection = ' '; // 'h' - horizontal, 'v' - vertical
    char lastShotDirection = ' '; // 'h' - horizontal, 'v' - vertical
    char lastShotDirectionAttempted = ' '; // 'h' - horizontal, 'v' - vertical
    int consecutiveHits = 0;
    char successfulShotDirection;
    bool reversedDirection;
};