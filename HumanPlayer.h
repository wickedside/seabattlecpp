#pragma once

#include "Player.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <cctype> // ��� ������� tolower
#include <limits> // ��� std::numeric_limit

class HumanPlayer : public Player {
public:
    HumanPlayer();
    ~HumanPlayer();

    std::pair<int, int> chooseShootCoordinate() override; // ����� ���������� ��� ��������
    void placeShips() override; // ���������� �������� ������ �� �����
    void autoPlaceShips();
    std::string getPlayerType() const override { return "�������"; } // ���������� ��� ������
private:
    Board ownBoard; // �����������, ��� � ��� ���� ����� Board.
};