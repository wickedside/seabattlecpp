#include "Game.h"
#include <iostream>

Game::Game() : human(), computer() {
    // Пусть игрок начинает первым
    currentPlayer = &human;
    opponent = &computer;
}

void Game::start() {
    std::cout << "Игра началась!" << std::endl;

    human.placeShips();
    computer.placeShips();

    while (!human.allShipsSunk() && !computer.allShipsSunk()) {
        std::cout << "Ход " << (currentPlayer == &human ? "игрока" : "компьютера") << std::endl;

        if (currentPlayer == &human) {
            std::cout << "Ваша доска:" << std::endl;
            human.getOwnBoard().display();
            std::cout << "\nДоска компьютера:" << std::endl;
            //human.getOpponentBoard().displayEnemyBoard();
        }
        else {
            // Это не обязательно, но если вы хотите отобразить доску компьютера (для отладки, например):
            std::cout << "Доска компьютера:" << std::endl;
            computer.getOwnBoard().display();
        }

        auto coordinate = currentPlayer->chooseShootCoordinate();
        ShipStatus status = currentPlayer->shootAtPlayer(*opponent, coordinate);

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
            break;
        }
        // Обработайте результат выстрела (промах, попадание, уничтожение корабля и т.д.)
        // Здесь можно добавить логику для отображения результата выстрела, например, вывод сообщений о попадании или промахе.

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
        std::cout << "Компьютер победил!" << std::endl;
    }
    else {
        std::cout << "Вы победили!" << std::endl;
    }
}