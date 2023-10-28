#include "Game.h"
#include <iostream>

ComputerPlayer::Mode convertToComputerMode(ShootingMode mode) {
    switch (mode) {
    case ShootingMode::RANDOM: return ComputerPlayer::Mode::RANDOM;
    case ShootingMode::INTELLIGENT: return ComputerPlayer::Mode::INTELLIGENT;
    default: return ComputerPlayer::Mode::RANDOM;
    }
}

Game::Game() : human(), computer1(), computer2() {
    gameMode = GameMode::HUMAN_VS_COMPUTER;
    placementMode = PlacementMode::MANUAL;
    shootingMode = ShootingMode::RANDOM;
}

void Game::start() {
    std::cout << "���� ��������!" << std::endl;

    if (gameMode == GameMode::HUMAN_VS_COMPUTER) {
        currentPlayer = &human;
        opponent = &computer1;

        computer1.setShootingMode(convertToComputerMode(shootingMode));

        if (placementMode == PlacementMode::MANUAL) {
            human.placeShips();
        }
        else {
            std::cout << "����������� ������� ������ �������������" << std::endl;
            human.autoPlaceShips(); // �������� ����� �������������� �����������
            std::cout << "������� ������ �����������" << std::endl;
            std::cout << "����� ������:" << std::endl;
            human.getOwnBoard().display();
        }
        
        // std::cout << "����������� ������� ���������� �������������" << std::endl;
        computer1.placeShips();

        // DEBUG
        /*
        std::cout << "������� ���������� �����������" << std::endl;
        std::cout << "����� ����������:" << std::endl;
        computer1.getOwnBoard().display();
 
        if (human.allShipsSunk()) {
            std::cout << "��� ������� ������ ��������� �� ������ ����!" << std::endl;
            return;
        }

        if (computer1.allShipsSunk()) {
            std::cout << "��� ������� ���������� ��������� �� ������ ����!" << std::endl;
            return;
        }*/

        humanVsComputerLoop();
    }
    else {
        currentPlayer = &computer1;
        opponent = &computer2;
        computer1.placeShips();
        computer2.placeShips();
        computerVsComputerLoop();
    }

    announceWinner();
}

void Game::humanVsComputerLoop() {
    while (!human.allShipsSunk() && !computer1.allShipsSunk()) {
        std::cout << "��� " << (currentPlayer == &human ? "������" : "����������") << std::endl;

        if (currentPlayer == &human) {
            std::cout << "���� �����:" << std::endl;
            human.getOwnBoard().display();
            std::cout << "\n����� ����������:" << std::endl;
            computer1.getOwnBoard().displayEnemyBoard();
        }

        auto coordinate = currentPlayer->chooseShootCoordinate();
        ShipStatus status = currentPlayer->shootAtPlayer(*opponent, coordinate);
        if (currentPlayer == &computer1) {  // ���������, ��� ������� ����� - ���������
            if (status == ShipStatus::HIT) {
                computer1.registerHit(coordinate.first, coordinate.second);
            }
            if (status == ShipStatus::SUNK && currentPlayer == &computer1) {
                computer1.registerMiss();
                computer1.resetInitialHit(); // �������� ���� ����� ��� ������ initialHit
            }
        }
        if (currentPlayer == &human) {
            human.updateOpponentBoard(computer1.getOwnBoard());
        }
        else {
            computer1.updateOpponentBoard(human.getOwnBoard());
        }

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
            switchPlayers();
            break;
        }
    }
}

void Game::computerVsComputerLoop() {
    while (!computer1.allShipsSunk() && !computer2.allShipsSunk()) {
        std::cout << "��� ����������" << (currentPlayer == &computer1 ? " 1" : " 2") << std::endl;

        std::cout << "����� ���������� 1:" << std::endl;
        computer1.getOwnBoard().display();
        std::cout << "\n����� ���������� 2:" << std::endl;
        computer2.getOwnBoard().display();

        auto coordinate = currentPlayer->chooseShootCoordinate();
        ShipStatus status = currentPlayer->shootAtPlayer(*opponent, coordinate);
        if (currentPlayer == &computer1) {
            computer1.updateOpponentBoard(computer2.getOwnBoard());
        }
        else {
            computer2.updateOpponentBoard(computer1.getOwnBoard());
        }

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
            switchPlayers();
            break;
        }
    }
}

void Game::switchPlayers() {
    if (gameMode == GameMode::HUMAN_VS_COMPUTER) {
        if (currentPlayer == &human) {
            currentPlayer = &computer1;
            opponent = &human;
        }
        else {
            currentPlayer = &human;
            opponent = &computer1;
        }
    }
    else {
        if (currentPlayer == &computer1) {
            currentPlayer = &computer2;
            opponent = &computer1;
        }
        else {
            currentPlayer = &computer1;
            opponent = &computer2;
        }
    }
}

void Game::announceWinner() {
    char startAgain;
    if (gameMode == GameMode::HUMAN_VS_COMPUTER) {
        if (human.allShipsSunk()) {
            std::cout << "��������� �������!" << std::endl;
        }
        else {
            std::cout << "�� ��������!" << std::endl;
        }
    }
    else {
        if (computer1.allShipsSunk()) {
            std::cout << "��������� 2 �������!" << std::endl;
        }
        else if (computer2.allShipsSunk()) {
            std::cout << "��������� 1 �������!" << std::endl;
        }
    }
    /*
    std::cout << "������ ���� ������? (y/n): ";
    std::cin >> startAgain;

    if (startAgain == 'y') {
        start();
    }
    else exit(0);
    */
}

/*void Game::handleStopMenu() {
    bool exitMenu = false;
    do {
        system("cls");
        std::cout << "1. ����������" << std::endl;
        std::cout << "2. ������ ���� ������" << std::endl;
        std::cout << "3. ����� � ������� ����" << std::endl;
        std::cout << "4. ����� �� ����" << std::endl;

        char choice = getCharWithoutEnter();
        switch (choice) {
        case '1':
            exitMenu = true;
            break;
        case '2':
            start();
            exitMenu = true;
            break;
        case '3':
            handleMenuSelection(*this);
            exitMenu = true;
            break;
        case '4':
            exit(0);
        default:
            std::cout << "�������� �����. ���������� ��� ���." << std::endl;
            break;
        }
    } while (!exitMenu);
}

void Game::handlePause() {
    system("cls");
    std::cout << "���� �� �����. ������� 'p', ����� ����������..." << std::endl;
    while (!isKeyPressed('p')) {
        // ��������...
    }
}

void Game::check() {
    if (isKeyPressed(27)) { // 27 - ��� ������� ESC
        handleStopMenu();
    }
    else if (isKeyPressed('p')) {
        handlePause();
    }
}*/