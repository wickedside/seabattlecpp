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
            std::cout << "[����������� ������� ������ �������������]" << std::endl;
            human.autoPlaceShips(); // �������� ����� �������������� �����������
            std::cout << "[������� ������ �����������]" << std::endl;
            //std::cout << "����� ������:" << std::endl;
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
        std::cout << "\n -> ��� " << (currentPlayer == &human ? "������\n" : "����������\n") << std::endl;

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
            else if (status == ShipStatus::SUNK) {
                computer1.resetShootingStrategy(); // ����� ��������� �������� ����� ����������� �������
                std::cout << "\n[����� ��������� �������� ����� ����������� �������]\n" << std::endl;
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
            std::cout << "\n[ ���������! ]\n" << std::endl;
            break;
        case ShipStatus::SUNK:
            std::cout << "\n[ ������� ���������! ]\n" << std::endl;
            break;
        case ShipStatus::ALIVE:
        default:
            std::cout << "\n[ ������! ]\n" << std::endl;
            switchPlayers();
            break;
        }
    }
}

void Game::computerVsComputerLoop() {
    while (!computer1.allShipsSunk() && !computer2.allShipsSunk()) {
        std::cout << "\n -> ��� ����������" << (currentPlayer == &computer1 ? " 1" : " 2") << std::endl;
        std::cout << "������� Enter ��� ���������� ����." << std::endl;

        bool enterPressed = false;
        while (!enterPressed) {
            if (_kbhit()) { // ���� ������ �������
                char keyPressed = _getch(); // �������� ��� �������
                if (keyPressed == 13) { // ��� ������� Enter
                    enterPressed = true;
                }
                else if (keyPressed == 'p' || keyPressed == 'P') { // ������� 'p' ��� 'P'
                    pauseGame();
                    std::cout << "������� Enter ��� ���������� ����." << std::endl;
                }
                else if (keyPressed == 27) { // ������� ESC
                    bool clear = true;
                    stopGame(clear);
                }
            }
        }

        std::cout << "����� ���������� 1:" << std::endl;
        computer1.getOwnBoard().display();
        std::cout << "\n����� ���������� 2:" << std::endl;
        computer2.getOwnBoard().display();

        auto coordinate = currentPlayer->chooseShootCoordinate();
        ShipStatus status = currentPlayer->shootAtPlayer(*opponent, coordinate);
        if (currentPlayer == &computer1) {  // ���������, ��� ������� ����� - ��������� 1
            if (status == ShipStatus::HIT) {
                computer1.registerHit(coordinate.first, coordinate.second);
            }
            else if (status == ShipStatus::SUNK) {
                computer1.resetShootingStrategy(); // ����� ��������� �������� ����� ����������� �������
                std::cout << "\n[����� ��������� �������� ����� ����������� �������]\n" << std::endl;
            }
        }
        else {  // ���������, ��� ������� ����� - ��������� 2
            if (status == ShipStatus::HIT) {
                computer2.registerHit(coordinate.first, coordinate.second);
            }
            else if (status == ShipStatus::SUNK) {
                computer2.resetShootingStrategy(); // ����� ��������� �������� ����� ����������� �������
                std::cout << "\n[����� ��������� �������� ����� ����������� �������]\n" << std::endl;
            }
        }
        // ���������� ����� ���������
        if (currentPlayer == &computer1) {
            computer1.updateOpponentBoard(computer2.getOwnBoard());
        }
        else {
            computer2.updateOpponentBoard(computer1.getOwnBoard());
        }

        switch (status) {
        case ShipStatus::HIT:
            std::cout << "\n[ ���������! ]\n" << std::endl;
            break;
        case ShipStatus::SUNK:
            std::cout << "\n[ ������� ���������! ]\n" << std::endl;
            break;
        case ShipStatus::ALIVE:
        default:
            std::cout << "\n[ ������! ]\n" << std::endl;
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
            std::cout << "\n[ ��������� �������! ]\n" << std::endl;
            std::cout << "\n����� ������:" << std::endl;
            human.getOwnBoard().display();
            std::cout << "\n����� ����������:" << std::endl;
            computer1.getOwnBoard().display();
        }
        else {
            std::cout << "\n[ �� ��������! ]\n" << std::endl;
            std::cout << "\n����� ������:" << std::endl;
            human.getOwnBoard().display();
            std::cout << "\n����� ����������:" << std::endl;
            computer1.getOwnBoard().display();
        }
    }
    else {
        if (computer1.allShipsSunk()) {
            std::cout << "\n[ ��������� 2 �������! ]\n" << std::endl;
            std::cout << "\n����� ���������� 1:" << std::endl;
            computer1.getOwnBoard().display();
            std::cout << "\n����� ���������� 2:" << std::endl;
            computer2.getOwnBoard().display();
        }
        else if (computer2.allShipsSunk()) {
            std::cout << "\n[ ��������� 1 �������! ]\n" << std::endl;
            std::cout << "\n����� ���������� 1:" << std::endl;
            computer1.getOwnBoard().display();
            std::cout << "\n����� ���������� 2:" << std::endl;
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
    if (_kbhit()) { // ��������, ������ �� �������
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
    // �������� ����� � ������� ��������� � �����
    system("cls"); // ����������� "clear" ���� ��������� �� Unix-�������� �������
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
    std::cout << "���� �� �����. ������� 'p', ����� ����������..." << std::endl;

    // �������� ���� �� ����� ������ ������� 'p'
    while (getCharWithoutEnter() != 'p') {
        // ����� ��� ����� ��������� ������ ������� ��� ������ �������
    }
    // ������������� ����, ��������, � ������������ ��������� ����
    // ��� ����� ������������� �������� ���������� �����, ������ � �.�.
}

void Game::stopGame(bool clear) {
    char choice;
    do {
        if (clear == true) {
            system("cls"); // �������� �����
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
        std::cout << "1. ����������" << std::endl;
        std::cout << "2. ����� � ������� ����" << std::endl;
        std::cout << "3. ����� �� ����" << std::endl;

        choice = getCharWithoutEnter();

        switch (choice) {
        case '1': // ����������� ����
            system("cls"); // �������� �����
            std::cout << "������� Enter, ����� ����������..." << std::endl;
            return;
        case '2': // ����� � ������� ����
            returnToMainMenu();
            return;
        case '3': // ����� �� ����
            exit(0);
            return;
        default: // ������������ ����, ��������� ������ ������
            std::cout << "�������� �����. ���������� ��� ���." << std::endl;
            break;
        }
    } while (true); // ����������� � ����, ���� �� ����� ������ ���������� �����
}

void Game::returnToMainMenu() {
    // ���������� ��������� ������� ����, ���� ����������
    // ����������� �������� ����
    Game seaBattle;
    MainMenu returnMenu;
    returnMenu.handleMenuSelection(seaBattle); // ��������������, ��� handleMenuSelection - ��� �������, ������� ���������� � ������������ ������� ����
}