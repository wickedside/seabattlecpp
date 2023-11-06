#include "Game.h"
#include <iostream>
#include "mainmenu.h"

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
    checkForPauseOrStop();
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
    checkForPauseOrStop();
    while (!human.allShipsSunk() && !computer1.allShipsSunk()) {
        checkForPauseOrStop();
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
        std::cout << "Нажмите Enter для следующего хода." << std::endl;

        bool enterPressed = false;
        while (!enterPressed) {
            if (_kbhit()) { // Если нажата клавиша
                char keyPressed = _getch(); // Получить код клавиши
                if (keyPressed == 13) { // Код клавиши Enter
                    enterPressed = true;
                }
                else if (keyPressed == 'p' || keyPressed == 'P') { // Клавиша 'p' или 'P'
                    pauseGame();
                    std::cout << "Нажмите Enter для следующего хода." << std::endl;
                }
                else if (keyPressed == 27) { // Клавиша ESC
                    bool clear = true;
                    stopGame(clear);
                }
            }
        }

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
    bool clear = false;
    stopGame(clear);
}

char getCharWithoutEnter() {
    return _getch();
}

void Game::checkForPauseOrStop() {
    if (_kbhit()) { // Проверка, нажата ли клавиша
        char keyPressed = getCharWithoutEnter();
        if (keyPressed == 'p') {
            pauseGame();
        }
        else if (keyPressed == 10 || keyPressed == 13) {
            //nextTurn();
        }
        else if (keyPressed == 27) { // ESC key
            bool clear = true;
            stopGame(clear);
        }
    }
}

void Game::pauseGame() {
    // Очистить экран и вывести сообщение о паузе
    system("cls"); // Используйте "clear" если работаете на Unix-подобной системе
    std::cout << R"(

 /$$$$$$$                                        
| $$__  $$                                       
| $$  \ $$ /$$$$$$  /$$   /$$  /$$$$$$$  /$$$$$$ 
| $$$$$$$/|____  $$| $$  | $$ /$$_____/ /$$__  $$
| $$____/  /$$$$$$$| $$  | $$|  $$$$$$ | $$$$$$$$
| $$      /$$__  $$| $$  | $$ \____  $$| $$_____/
| $$     |  $$$$$$$|  $$$$$$/ /$$$$$$$/|  $$$$$$$
|__/      \_______/ \______/ |_______/  \_______/
                                                                                                                                 
)" << std::endl;
    std::cout << "Игра на паузе. Нажмите 'p', чтобы продолжить..." << std::endl;

    // Ожидание пока не будет нажата клавиша 'p'
    while (getCharWithoutEnter() != 'p') {
        // Здесь код может выполнять другие функции или просто ожидать
    }
    // Возобновление игры, возможно, с перерисовкой элементов игры
    // Вам может потребоваться повторно отобразить доски, статус и т.д.
}

void Game::stopGame(bool clear) {
    char choice;
    do {
        if (clear == true) {
            system("cls"); // Очистить экран
        }
        std::cout << R"(

 /$$      /$$                              
| $$$    /$$$                              
| $$$$  /$$$$  /$$$$$$  /$$$$$$$  /$$   /$$
| $$ $$/$$ $$ /$$__  $$| $$__  $$| $$  | $$
| $$  $$$| $$| $$$$$$$$| $$  \ $$| $$  | $$
| $$\  $ | $$| $$_____/| $$  | $$| $$  | $$
| $$ \/  | $$|  $$$$$$$| $$  | $$|  $$$$$$/
|__/     |__/ \_______/|__/  |__/ \______/ 
                                                                            
)" << std::endl;
        std::cout << "1. Продолжить" << std::endl;
        std::cout << "2. Выйти в главное меню" << std::endl;
        std::cout << "3. Выйти из игры" << std::endl;

        choice = getCharWithoutEnter();

        switch (choice) {
        case '1': // Продолжение игры
            system("cls"); // Очистить экран
            std::cout << "Нажмите Enter, чтобы продолжить..." << std::endl;
            return;
        case '2': // Выход в главное меню
            returnToMainMenu();
            return;
        case '3': // Выход из игры
            exit(0);
            return;
        default: // Недопустимый ввод, повторите запрос выбора
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
            break;
        }
    } while (true); // Оставайтесь в меню, пока не будет сделан допустимый выбор
}

void Game::returnToMainMenu() {
    // Сохранение состояния текущей игры, если необходимо
    // Отображение главного меню
    Game seaBattle;
    MainMenu returnMenu;
    returnMenu.handleMenuSelection(seaBattle); // предполагается, что handleMenuSelection - это функция, которая отображает и обрабатывает главное меню
}