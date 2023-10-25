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

private:
    enum class Mode { RANDOM, INTELLIGENT };

    Mode shootingMode = Mode::RANDOM;
    std::vector<std::pair<int, int>> intelligentTargets; // ��������� ���� � ���������������� ������

    void generateRandomShips();
    bool canPlaceShip(const int x, const int y, const char direction, const int size);

    std::pair<int, int> lastHit; // ��������� ���������
    bool shipDirectionDetermined = false; // ����������� ������� ����������
    char shipDirection; // 'h' ��� 'v'

    void addSurroundingCoordinates(int x, int y);
    void resetShootingStrategy();
};
