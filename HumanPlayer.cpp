#include "Board.h"
#include "HumanPlayer.h"

HumanPlayer::HumanPlayer() {
    // Инициализация, если необходимо
}

HumanPlayer::~HumanPlayer() {
    // Освобождение ресурсов, если необходимо
}

std::pair<int, int> HumanPlayer::chooseShootCoordinate() {
    int x = -1, y = -1;
    do {
        // Запрос координаты X
        while (true) {
            std::cout << "\n[?] Введите координату X (0-9) для выстрела: ";
            char ch = _getch(); // считываем символ без echo
            if (isdigit(ch) && ch >= '0' && ch <= '9') { // проверка, что это цифра от 0 до 9
                x = ch - '0'; // преобразование char в int
                std::cout << x << std::endl; // выводим выбранное число для подтверждения пользователю
                break; // выходим из цикла, если ввод корректен
            }
        }

        // Запрос координаты Y
        while (true) {
            std::cout << "\n[?] Введите координату Y (0-9) для выстрела: ";
            char ch = _getch(); // считываем символ без echo
            if (isdigit(ch) && ch >= '0' && ch <= '9') { // проверка, что это цифра от 0 до 9
                y = ch - '0'; // преобразование char в int
                std::cout << y << std::endl; // выводим выбранное число для подтверждения пользователю
                break; // выходим из цикла, если ввод корректен
            }
        }
        if (opponentBoard.isCellShot(x, y)) {
            std::cout << "\n[!] Вы уже стреляли в эти координаты или эта клетка помечена как промах. Попробуйте еще раз.\n";
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
            std::cout << "\n -> Текущая расстановка кораблей на доске:\n";
            ownBoard.display();  // Выводим текущее состояние доски
            std::cout << "\n";

            std::cout << "\n -> Разместите " << description << " корабль (" << (i + 1) << " из " << count << "):\n";

            int x = -1, y = -1;
            char direction = '\0';

            // Запрос координаты X
            while (true) {
                std::cout << "\n[?] Введите начальную координату X (0-9): ";
                char ch = _getch(); // считываем символ без echo
                if (isdigit(ch) && ch >= '0' && ch <= '9') { // проверка, что это цифра от 0 до 9
                    x = ch - '0'; // преобразование char в int
                    std::cout << x << std::endl; // выводим выбранное число для подтверждения пользователю
                    break; // выходим из цикла, если ввод корректен
                }
            }

            // Запрос координаты Y
            while (true) {
                std::cout << "\n[?] Введите начальную координату Y (0-9): ";
                char ch = _getch(); // считываем символ без echo
                if (isdigit(ch) && ch >= '0' && ch <= '9') { // проверка, что это цифра от 0 до 9
                    y = ch - '0'; // преобразование char в int
                    std::cout << y << std::endl; // выводим выбранное число для подтверждения пользователю
                    break; // выходим из цикла, если ввод корректен
                }
            }

            // Запрос ориентации корабля
            while (true) {
                std::cout << "\n[?] Введите направление корабля (h - горизонтальное, v - вертикальное): ";
                char ch = _getch(); // считываем символ без echo
                ch = tolower(ch); // приводим к нижнему регистру для унификации
                if (ch == 'h' || ch == 'v') { // проверка, что ввод это 'h' или 'v'
                    direction = ch;
                    std::cout << direction << std::endl; // выводим выбранное направление для подтверждения пользователю
                    break; // выходим из цикла, если ввод корректен
                }
            }

            // Используйте ownBoard для вызова методов canPlaceShip и placeShip
            if (ownBoard.canPlaceShip(x, y, direction, size)) {
                ownBoard.placeShip(x, y, direction, size);
                isPlaced = true;
            }
            else {
                std::cout << "[!] Не удается разместить корабль в выбранных координатах. Попробуйте еще раз.\n";
            }
        }
    }
}

void HumanPlayer::autoPlaceShips() {
    autoPlaceShipsBasic();
}