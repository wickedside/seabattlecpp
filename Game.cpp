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
   std::cout << R"(

  /$$$$$$  /$$$$$$$$ /$$$$$$  /$$$$$$$  /$$$$$$$$
 /$$__  $$|__  $$__//$$__  $$| $$__  $$|__  $$__/
| $$  \__/   | $$  | $$  \ $$| $$  \ $$   | $$   
|  $$$$$$    | $$  | $$$$$$$$| $$$$$$$/   | $$   
 \____  $$   | $$  | $$__  $$| $$__  $$   | $$   
 /$$  \ $$   | $$  | $$  | $$| $$  \ $$   | $$   
|  $$$$$$/   | $$  | $$  | $$| $$  | $$   | $$   
 \______/    |__/  |__/  |__/|__/  |__/   |__/   
                                                                                                                                     
)" << std::endl;

    if (gameMode == GameMode::HUMAN_VS_COMPUTER) {
        currentPlayer = &human;
        opponent = &computer1;

        computer1.setShootingMode(convertToComputerMode(shootingMode));

        if (placementMode == PlacementMode::MANUAL) {
            human.placeShips();
        }
        else {
            std::cout << "[Расставляем корабли игрока автоматически]" << std::endl;
            human.autoPlaceShips(); // вызываем метод автоматической расстановки
            std::cout << "[Корабли игрока расставлены]" << std::endl;
            //std::cout << "Доска игрока:" << std::endl;
            //human.getOwnBoard().display();
        }
        
        computer1.placeShips();
        humanVsComputerLoop();
    }
    else {
        currentPlayer = &computer1;
        opponent = &computer2;
        computer1.setShootingMode(convertToComputerMode(shootingMode));
        computer2.setShootingMode(convertToComputerMode(shootingMode));
        computer1.placeShips();
        computer2.placeShips();
        computerVsComputerLoop();
    }
    announceWinner();
}

void Game::humanVsComputerLoop() {
    while (!human.allShipsSunk() && !computer1.allShipsSunk()) {
        std::cout << "\n -> Ход " << (currentPlayer == &human ? "игрока\n" : "компьютера\n") << std::endl;

        if (currentPlayer == &human) {
            std::cout << "Ваша доска:" << std::endl;
            human.getOwnBoard().display();
            std::cout << "\nДоска компьютера:" << std::endl;
            computer1.getOwnBoard().displayEnemyBoard();
        }

        auto coordinate = currentPlayer->chooseShootCoordinate();
        ShipStatus status = currentPlayer->shootAtPlayer(*opponent, coordinate);
        if (currentPlayer == &computer1) {  // Проверяем, что текущий игрок - компьютер
            if (status == ShipStatus::HIT) {
                computer1.registerHit(coordinate.first, coordinate.second);
            }
            else if (status == ShipStatus::SUNK) {
                computer1.resetShootingStrategy(); // Сброс стратегии стрельбы после уничтожения корабля
                std::cout << "\n[СБРОС СТРАТЕГИИ СТРЕЛЬБЫ ПОСЛЕ УНИЧТОЖЕНИЯ КОРАБЛЯ]\n" << std::endl;
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
            std::cout << "\n[ Попадание! ]\n" << std::endl;
            break;
        case ShipStatus::SUNK:
            std::cout << "\n[ Корабль уничтожен! ]\n" << std::endl;
            break;
        case ShipStatus::ALIVE:
        default:
            std::cout << "\n[ Промах! ]\n" << std::endl;
            switchPlayers();
            break;
        }
    }
}

void Game::computerVsComputerLoop() {
    while (!computer1.allShipsSunk() && !computer2.allShipsSunk()) {
        std::cout << "\n -> Ход компьютера" << (currentPlayer == &computer1 ? " 1" : " 2") << std::endl;

        std::cout << "Доска компьютера 1:" << std::endl;
        computer1.getOwnBoard().display();
        std::cout << "\nДоска компьютера 2:" << std::endl;
        computer2.getOwnBoard().display();

        auto coordinate = currentPlayer->chooseShootCoordinate();
        ShipStatus status = currentPlayer->shootAtPlayer(*opponent, coordinate);
        if (currentPlayer == &computer1) {  // Проверяем, что текущий игрок - компьютер 1
            if (status == ShipStatus::HIT) {
                computer1.registerHit(coordinate.first, coordinate.second);
            }
            else if (status == ShipStatus::SUNK) {
                computer1.resetShootingStrategy(); // Сброс стратегии стрельбы после уничтожения корабля
                std::cout << "\n[СБРОС СТРАТЕГИИ СТРЕЛЬБЫ ПОСЛЕ УНИЧТОЖЕНИЯ КОРАБЛЯ]\n" << std::endl;
            }
        }
        else {  // Проверяем, что текущий игрок - компьютер 2
            if (status == ShipStatus::HIT) {
                computer2.registerHit(coordinate.first, coordinate.second);
            }
            else if (status == ShipStatus::SUNK) {
                computer2.resetShootingStrategy(); // Сброс стратегии стрельбы после уничтожения корабля
                std::cout << "\n[СБРОС СТРАТЕГИИ СТРЕЛЬБЫ ПОСЛЕ УНИЧТОЖЕНИЯ КОРАБЛЯ]\n" << std::endl;
            }
        }
        // Обновление доски оппонента
        if (currentPlayer == &computer1) {
            computer1.updateOpponentBoard(computer2.getOwnBoard());
        }
        else {
            computer2.updateOpponentBoard(computer1.getOwnBoard());
        }

        switch (status) {
        case ShipStatus::HIT:
            std::cout << "\n[ Попадание! ]\n" << std::endl;
            break;
        case ShipStatus::SUNK:
            std::cout << "\n[ Корабль уничтожен! ]\n" << std::endl;
            break;
        case ShipStatus::ALIVE:
        default:
            std::cout << "\n[ Промах! ]\n" << std::endl;
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
            std::cout << "\n[ Компьютер победил! ]\n" << std::endl;
            std::cout << "\nДоска игрока:" << std::endl;
            human.getOwnBoard().display();
            std::cout << "\nДоска компьютера:" << std::endl;
            computer1.getOwnBoard().display();
        }
        else {
            std::cout << "\n[ Вы победили! ]\n" << std::endl;
            std::cout << "\nДоска игрока:" << std::endl;
            human.getOwnBoard().display();
            std::cout << "\nДоска компьютера:" << std::endl;
            computer1.getOwnBoard().display();
        }
    }
    else {
        if (computer1.allShipsSunk()) {
            std::cout << "\n[ Компьютер 2 победил! ]\n" << std::endl;
            std::cout << "\nДоска компьютера 1:" << std::endl;
            computer1.getOwnBoard().display();
            std::cout << "\nДоска компьютера 2:" << std::endl;
            computer2.getOwnBoard().display();
        }
        else if (computer2.allShipsSunk()) {
            std::cout << "\n[ Компьютер 1 победил! ]\n" << std::endl;
            std::cout << "\nДоска компьютера 1:" << std::endl;
            computer1.getOwnBoard().display();
            std::cout << "\nДоска компьютера 2:" << std::endl;
            computer2.getOwnBoard().display();
        }
    }
    /*
    std::cout << "Начать игру заново? (y/n): ";
    std::cin >> startAgain;

    if (startAgain == 'y') {
        start();
    }
    */
    exit(0);
}

/*void Game::handleStopMenu() {
    bool exitMenu = false;
    do {
        system("cls");
        std::cout << "1. Продолжить" << std::endl;
        std::cout << "2. Начать игру заново" << std::endl;
        std::cout << "3. Выйти в главное меню" << std::endl;
        std::cout << "4. Выйти из игры" << std::endl;

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
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
            break;
        }
    } while (!exitMenu);
}

void Game::handlePause() {
    system("cls");
    std::cout << "Игра на паузе. Нажмите 'p', чтобы продолжить..." << std::endl;
    while (!isKeyPressed('p')) {
        // Ожидание...
    }
}

void Game::check() {
    if (isKeyPressed(27)) { // 27 - код клавиши ESC
        handleStopMenu();
    }
    else if (isKeyPressed('p')) {
        handlePause();
    }
}*/