#include "Game.h"

void displayMenu() {
    std::cout << "1. Выберите режим игры:" << std::endl;
    std::cout << "2. Как расставить корабли?" << std::endl;
    std::cout << "3. Выберите режим стрельбы для компьютера:" << std::endl;
    std::cout << "4. Начать игру" << std::endl;
}

void handleMenuSelection(Game& game) {
    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::cout << "    1. Человек против Компьютера" << std::endl;
            std::cout << "    2. Компьютер против Компьютера" << std::endl;
            int modeChoice;
            std::cin >> modeChoice;
            if (modeChoice == 1) {
                game.setGameMode(GameMode::HUMAN_VS_COMPUTER);
            }
            else if (modeChoice == 2) {
                game.setGameMode(GameMode::COMPUTER_VS_COMPUTER);
            }
            break;
        }
        case 2: {
            std::cout << "    1. Ручное расставление" << std::endl;
            std::cout << "    2. Автоматическое расставление" << std::endl;
            int placementChoice;
            std::cin >> placementChoice;
            if (placementChoice == 1) {
                game.setPlacementMode(PlacementMode::MANUAL);
            }
            else if (placementChoice == 2) {
                game.setPlacementMode(PlacementMode::AUTO);
            }
            break;
        }
        case 3: {
            std::cout << "    1. Случайные выстрелы" << std::endl;
            std::cout << "    2. Интеллектуальные выстрелы" << std::endl;
            int shootingChoice;
            std::cin >> shootingChoice;
            if (shootingChoice == 1) {
                game.setShootingMode(ShootingMode::RANDOM);
            }
            else if (shootingChoice == 2) {
                game.setShootingMode(ShootingMode::INTELLIGENT);
            }
            break;
        }
        case 4:
            game.start();
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
            break;
        }
    } while (choice != 4);
}

int main() {
    setlocale(LC_ALL, "");
    Game seaBattle;
    handleMenuSelection(seaBattle);
    return 0;
}