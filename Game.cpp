#include "Game.h"
#include <iostream>

Game::Game() : human(), computer() {
    // ����� ����� �������� ������
    currentPlayer = &human;
    opponent = &computer;
}

void Game::start() {
    std::cout << "���� ��������!" << std::endl;

    human.placeShips();
    computer.placeShips();

    while (!human.allShipsSunk() && !computer.allShipsSunk()) {
        std::cout << "��� " << (currentPlayer == &human ? "������" : "����������") << std::endl;

        if (currentPlayer == &human) {
            std::cout << "���� �����:" << std::endl;
            human.getOwnBoard().display();
            std::cout << "\n����� ����������:" << std::endl;
            //human.getOpponentBoard().displayEnemyBoard();
        }
        else {
            // ��� �� �����������, �� ���� �� ������ ���������� ����� ���������� (��� �������, ��������):
            std::cout << "����� ����������:" << std::endl;
            computer.getOwnBoard().display();
        }

        auto coordinate = currentPlayer->chooseShootCoordinate();
        ShipStatus status = currentPlayer->shootAtPlayer(*opponent, coordinate);

        switch (status) {
        case ShipStatus::HIT:
            std::cout << "���������!" << std::endl;
            break;
        case ShipStatus::SUNK:
            std::cout << "������� ���������!" << std::endl;
            break;
        case ShipStatus::ALIVE:
        default:
            std::cout << "������!" << std::endl;
            break;
        }
        // ����������� ��������� �������� (������, ���������, ����������� ������� � �.�.)
        // ����� ����� �������� ������ ��� ����������� ���������� ��������, ��������, ����� ��������� � ��������� ��� �������.

        switchPlayers();
    }

    announceWinner();
}

void Game::switchPlayers() {
    if (currentPlayer == &human) {
        currentPlayer = &computer;
        opponent = &human;
    }
    else {
        currentPlayer = &human;
        opponent = &computer;
    }
}

void Game::announceWinner() {
    if (human.allShipsSunk()) {
        std::cout << "��������� �������!" << std::endl;
    }
    else {
        std::cout << "�� ��������!" << std::endl;
    }
}