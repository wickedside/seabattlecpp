#include "Board.h"
#include "HumanPlayer.h"

HumanPlayer::HumanPlayer() {
    // Инициализация, если необходимо
}

HumanPlayer::~HumanPlayer() {
    // Освобождение ресурсов, если необходимо
}

std::pair<int, int> HumanPlayer::chooseShootCoordinate() {
    int x, y;
    do {
        std::cout << "Введите координату X для выстрела (0-9): ";
        std::cin >> x;
        std::cout << "Введите координату Y для выстрела (0-9): ";
        std::cin >> y;
        if (opponentBoard.isCellShot(x, y)) {
            std::cout << "Вы уже стреляли в эти координаты или эта клетка помечена как промах. Попробуйте еще раз.\n";
        }
    } while (opponentBoard.isCellShot(x, y));

    return std::make_pair(x, y);
}

void HumanPlayer::placeShips() {
    std::cout << "Размещение кораблей на доске:\n";
    // Размещение кораблей в соответствии с их количеством и размером
    placeShip(4, 1, "четырёхпалубный");
    placeShip(3, 2, "трёхпалубный");
    placeShip(2, 3, "двухпалубный");
    placeShip(1, 4, "однопалубный");
}

void Player::placeShip(int size, int count, const std::string& description) {
    for (int i = 0; i < count; ++i) {
        bool isPlaced = false;
        while (!isPlaced) {
            std::cout << "Текущая расстановка кораблей на доске:\n";
            ownBoard.display();  // Выводим текущее состояние доски
            std::cout << "\n";

            std::cout << "Разместите " << description << " корабль (" << (i + 1) << " из " << count << "):\n";

            int x, y;
            char direction;

            std::cout << "Введите начальную координату X (0-9): ";
            std::cin >> x;
            std::cout << "Введите начальную координату Y (0-9): ";
            std::cin >> y;
            std::cout << "Введите направление корабля (h - горизонтальное, v - вертикальное): ";
            std::cin >> direction;

            // Используйте ownBoard для вызова методов canPlaceShip и placeShip
            if (ownBoard.canPlaceShip(x, y, direction, size)) {
                ownBoard.placeShip(x, y, direction, size);
                isPlaced = true;
            }
            else {
                std::cout << "Не удается разместить корабль в выбранных координатах. Попробуйте еще раз.\n";
            }
        }
    }
}