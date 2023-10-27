#include "Game.h"
#include <conio.h>

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

char getCharWithoutEnter() {
    return _getch();
}

void displayMenu() {
    system("cls");
    seaBattleLogo();
    std::cout << "1. Начать игру." << std::endl;
    std::cout << "2. Выбрать режим игры." << std::endl;
    std::cout << "3. Выбрать расстановку кораблей." << std::endl;
    std::cout << "4. Выбрать режим стрельбы для компьютера." << std::endl;
    std::cout << "5. Выйти из игры." << std::endl;
}

void handleMenuSelection(Game& game) {
    int choice;
    do {
        displayMenu();
        choice = getCharWithoutEnter();

        switch (choice) {
        case '1':
            system("cls");
            game.start();
            break;
        case '2': {
            system("cls");
            seaBattleLogo();
            std::cout << "Выберите режим игры:" << std::endl;
            std::cout << "    1. Человек против Компьютера" << std::endl;
            std::cout << "    2. Компьютер против Компьютера" << std::endl;
            int modeChoice;
            modeChoice = getCharWithoutEnter();
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
            std::cout << "Выберите расстановку кораблей:" << std::endl;
            std::cout << "    1. Ручное расставление" << std::endl;
            std::cout << "    2. Автоматическое расставление" << std::endl;
            int placementChoice;
            placementChoice = getCharWithoutEnter();
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
            std::cout << "Выберите режим стрельбы для компьютера:" << std::endl;
            std::cout << "    1. Случайные выстрелы" << std::endl;
            std::cout << "    2. Интеллектуальные выстрелы" << std::endl;
            int shootingChoice;
            shootingChoice = getCharWithoutEnter();
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
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
            break;
        }
    } while (choice != '5');
}

int main() {
    setlocale(LC_ALL, "");
    Game seaBattle;
    handleMenuSelection(seaBattle);
    return 0;
}

/*
bool isKeyPressed(int key) {
    return _kbhit() && _getch() == key;
}
*/