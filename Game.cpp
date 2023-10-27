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
    std::cout << "Игра началась!" << std::endl;

    if (gameMode == GameMode::HUMAN_VS_COMPUTER) {
        currentPlayer = &human;
        opponent = &computer1;
        if (placementMode == PlacementMode::MANUAL) {
            human.placeShips();
        }
        else {
            human.placeShips();
        }
        computer1.placeShips();
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
        std::cout << "Ход " << (currentPlayer == &human ? "игрока" : "компьютера") << std::endl;

        if (currentPlayer == &human) {
            std::cout << "Ваша доска:" << std::endl;
            human.getOwnBoard().display();
            std::cout << "\nДоска компьютера:" << std::endl;
            computer1.getOwnBoard().displayEnemyBoard();
        }

        auto coordinate = currentPlayer->chooseShootCoordinate();
        ShipStatus status = currentPlayer->shootAtPlayer(*opponent, coordinate);
        if (currentPlayer == &human) {
            human.updateOpponentBoard(computer1.getOwnBoard());
        }
        else {
            computer1.updateOpponentBoard(human.getOwnBoard());
        }

        switch (status) {
        case ShipStatus::HIT:
            std::cout << "Попадание!" << std::endl;
            break;
        case ShipStatus::SUNK:
            std::cout << "Корабль уничтожен!" << std::endl;
            break;
        case ShipStatus::ALIVE:
        default:
            std::cout << "Промах!" << std::endl;
            switchPlayers();
            break;
        }
    }
}

void Game::computerVsComputerLoop() {
    while (!computer1.allShipsSunk() && !computer2.allShipsSunk()) {
        std::cout << "Ход компьютера" << (currentPlayer == &computer1 ? " 1" : " 2") << std::endl;

        std::cout << "Доска компьютера 1:" << std::endl;
        computer1.getOwnBoard().display();
        std::cout << "\nДоска компьютера 2:" << std::endl;
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
            std::cout << "Попадание!" << std::endl;
            break;
        case ShipStatus::SUNK:
            std::cout << "Корабль уничтожен!" << std::endl;
            break;
        case ShipStatus::ALIVE:
        default:
            std::cout << "Промах!" << std::endl;
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
    if (gameMode == GameMode::HUMAN_VS_COMPUTER) {
        if (human.allShipsSunk()) {
            std::cout << "Компьютер победил!" << std::endl;
        }
        else {
            std::cout << "Вы победили!" << std::endl;
        }
    }
    else {
        if (computer1.allShipsSunk()) {
            std::cout << "Компьютер 2 победил!" << std::endl;
        }
        else if (computer2.allShipsSunk()) {
            std::cout << "Компьютер 1 победил!" << std::endl;
        }
    }
}