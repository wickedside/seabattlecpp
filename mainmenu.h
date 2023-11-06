#pragma once

#include "Game.h"
#include <conio.h>

class MainMenu {
public:
    void seaBattleLogo() {
        std::cout << R"(

  /$$$$$$                            /$$$$$$$              /$$     /$$     /$$          
 /$$__  $$                          | $$__  $$            | $$    | $$    | $$          
| $$  \__/  /$$$$$$   /$$$$$$       | $$  \ $$  /$$$$$$  /$$$$$$ /$$$$$$  | $$  /$$$$$$ 
|  $$$$$$  /$$__  $$ |____  $$      | $$$$$$$  |____  $$|_  $$_/|_  $$_/  | $$ /$$__  $$
 \____  $$| $$$$$$$$  /$$$$$$$      | $$__  $$  /$$$$$$$  | $$    | $$    | $$| $$$$$$$$
 /$$  \ $$| $$_____/ /$$__  $$      | $$  \ $$ /$$__  $$  | $$ /$$| $$ /$$| $$| $$_____/
|  $$$$$$/|  $$$$$$$|  $$$$$$$      | $$$$$$$/|  $$$$$$$  |  $$$$/|  $$$$/| $$|  $$$$$$$
 \______/  \_______/ \_______/      |_______/  \_______/   \___/   \___/  |__/ \_______/
                                                                                        
)" << std::endl;
    }

    void displayMenu() {
        system("cls");
        seaBattleLogo();
        std::cout << "1. ������ ����." << std::endl;
        std::cout << "2. ������� ����� ����." << std::endl;
        std::cout << "3. ������� ����������� ��������." << std::endl;
        std::cout << "4. ������� ����� �������� ��� ����������." << std::endl;
        std::cout << "5. ����� �� ����." << std::endl;
    }

    char getCharWith0utEnter() {
        return _getch();
    }

    void handleMenuSelection(Game& game) {
        int choice;
        do {
            displayMenu();
            choice = getCharWith0utEnter();

            switch (choice) {
            case '1':
                system("cls");
                game.start();
                break;
            case '2': {
                system("cls");
                seaBattleLogo();
                std::cout << "�������� ����� ����:" << std::endl;
                std::cout << "    1. ������� ������ ����������" << std::endl;
                std::cout << "    2. ��������� ������ ����������" << std::endl;
                int modeChoice;
                modeChoice = getCharWith0utEnter();
                if (modeChoice == '1') {
                    game.setGameMode(GameMode::HUMAN_VS_COMPUTER);
                }
                else if (modeChoice == '2') {
                    game.setGameMode(GameMode::COMPUTER_VS_COMPUTER);
                }
                break;
            }
            case '3': {
                system("cls");
                seaBattleLogo();
                std::cout << "�������� ����������� ��������:" << std::endl;
                std::cout << "    1. ������ ������������" << std::endl;
                std::cout << "    2. �������������� ������������" << std::endl;
                int placementChoice;
                placementChoice = getCharWith0utEnter();
                if (placementChoice == '1') {
                    game.setPlacementMode(PlacementMode::MANUAL);
                }
                else if (placementChoice == '2') {
                    game.setPlacementMode(PlacementMode::AUTO);
                }
                break;
            }
            case '4': {
                system("cls");
                seaBattleLogo();
                std::cout << "�������� ����� �������� ��� ����������:" << std::endl;
                std::cout << "    1. ��������� ��������" << std::endl;
                std::cout << "    2. ���������������� ��������" << std::endl;
                int shootingChoice;
                shootingChoice = getCharWith0utEnter();
                if (shootingChoice == '1') {
                    game.setShootingMode(ShootingMode::RANDOM);
                }
                else if (shootingChoice == '2') {
                    game.setShootingMode(ShootingMode::INTELLIGENT);
                }
                break;
            }
            case '5':
                exit(0);
            default:
                std::cout << "�������� �����. ���������� ��� ���." << std::endl;
                break;
            }
        } while (choice != '5');
    }
};
