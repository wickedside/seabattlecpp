#pragma once

#include "Player.h"
#include <cstdlib> // ��� rand()
#include <vector>

class ComputerPlayer : public Player {
public:
    ComputerPlayer();
    ~ComputerPlayer();

    std::pair<int, int> chooseShootCoordinate() override;
    void placeShips() override;
    std::string getPlayerType() const override { return "���������"; }

    enum class Mode { RANDOM, INTELLIGENT };
    void setShootingMode(Mode mode);
    std::pair<int, int> randomShoot();
    std::pair<int, int> shootAroundLastHit();
    std::pair<int, int> shootInDirection();

private:
    Mode shootingMode = Mode::RANDOM;
    std::vector<std::pair<int, int>> intelligentTargets; // ��������� ���� � ���������������� ������

    void autoPlaceShips();
    bool canPlaceShip(const int x, const int y, const char direction, const int size);

    bool shipDirectionDetermined = false; // ����������� ������� ����������
    char shipDirection; // 'h' ��� 'v'

    void addSurroundingCoordinates(int x, int y);
    void resetShootingStrategy();
    std::pair<int, int> lastHit = { -1, -1 };
    char lastHitDirection = ' '; // 'h' - horizontal, 'v' - vertical
    int consecutiveHits = 0;
};
