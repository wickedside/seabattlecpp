#include "Board.h"
#include <iostream>

Board::Board() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            grid[i][j] = ' ';  // начальная инициализация доски пустыми клетками
        }
    }
}

/*bool Board::isValidPlacement(const Ship& ship) const {
    int x = ship.getStartCoordinate().first;
    int y = ship.getStartCoordinate().second;
    int len = ship.getLength();

    if (ship.getDirection() == Direction::HORIZONTAL) {
        if (y + len > BOARD_SIZE) return false;

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j < len + 1; j++) {
                if (x + i >= 0 && x + i < BOARD_SIZE && y + j >= 0 && y + j < BOARD_SIZE) {
                    if (grid[x + i][y + j] != ' ') return false;
                }
            }
        }
    }
    else {
        if (x + len > BOARD_SIZE) return false;

        for (int i = -1; i < len + 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (x + i >= 0 && x + i < BOARD_SIZE && y + j >= 0 && y + j < BOARD_SIZE) {
                    if (grid[x + i][y + j] != ' ') return false;
                }
            }
        }
    }

    return true;
}*/

bool Board::addShip(const Ship& ship) {
    int x = ship.getStartCoordinate().first;
    int y = ship.getStartCoordinate().second;
    int len = ship.getLength();
    char dir = (ship.getDirection() == Direction::HORIZONTAL) ? 'h' : 'v';

    if (!canPlaceShip(x, y, dir, len)) return false;

    ships.push_back(ship);

    // Отладочная печать
    std::cout << "Added ship at: (" << ship.getStartCoordinate().first << ", " << ship.getStartCoordinate().second << ") with length: " << ship.getLength() << std::endl;

    int curX = x; // новая переменная для индекса столбца
    int curY = y; // новая переменная для индекса строки

    if (ship.getDirection() == Direction::HORIZONTAL) {
        for (int j = 0; j < len; j++) {
            grid[curY][curX + j] = 'O';  // O обозначает корабль на доске
        }
    }
    else {
        for (int i = 0; i < len; i++) {
            grid[curY + i][curX] = 'O';
        }
    }

    return true;
}

ShipStatus Board::shootAt(const std::pair<int, int>& coordinate) {
    int x = coordinate.first;
    int y = coordinate.second;

    if (grid[y][x] == 'X') { // Используем y перед x
        return ShipStatus::HIT;
    }
    if (grid[y][x] == '.') { // Используем y перед x
        return ShipStatus::ALIVE;
    }

    for (Ship& ship : ships) {
        if (ship.isAt(coordinate)) {
            if (ship.hit(coordinate)) {
                grid[y][x] = 'X';  // Используем y перед x
                std::cout << "Hit ship at: (" << ship.getStartCoordinate().first << ", " << ship.getStartCoordinate().second << ") with length: " << ship.getLength() << std::endl;
                if (ship.getStatus() == ShipStatus::SUNK) {
                    markSurroundingAsMiss(ship);
                    return ShipStatus::SUNK;
                }
                return ShipStatus::HIT;
            }
        }
    }
    grid[y][x] = '.';  // Используем y перед x
    return ShipStatus::ALIVE;
}

char Board::getCell(const std::pair<int, int>& coordinate) const {
    return grid[coordinate.first][coordinate.second];
}

void Board::display() const {
    // Выводим шапку
    std::cout << "[ ]|0|1|2|3|4|5|6|7|8|9|" << std::endl;

    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << "[" << i << "]|";  // Выводим номер строки
        for (int j = 0; j < BOARD_SIZE; j++) {
            std::cout << grid[i][j] << '|';
        }
        std::cout << std::endl;
    }
}

void Board::displayEnemyBoard() const {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (grid[i][j] == 'X' || grid[i][j] == '.') {
                std::cout << grid[i][j] << ' ';
            }
            else {
                std::cout << ' ' << ' ';
            }
        }
        std::cout << std::endl;
    }
}

const std::vector<Ship>& Board::getShips() const {
    return ships;
}

bool Board::canPlaceShip(int x, int y, char direction, int size) const {
    // Проверка на выход за границы доски
    if (direction == 'h' && x + size > BOARD_SIZE) return false;
    if (direction == 'v' && y + size > BOARD_SIZE) return false;


    for (int i = 0; i < size; ++i) {
        // Проверка занятости ячейки
        if (grid[y][x] != ' ') return false;  // Используем ' ' вместо CellStatus::EMPTY

        // Проверка соседних ячеек (по вертикали и горизонтали)
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int newY = y + dy;
                int newX = x + dx;

                // Если координаты внутри доски и ячейка не пустая
                if (newY >= 0 && newY < 10 && newX >= 0 && newX < 10 && grid[newY][newX] != ' ') {  // Используем ' ' вместо CellStatus::EMPTY
                    // Исключим текущую ячейку из проверки
                    if (dy == 0 && dx == 0) continue;
                    return false;
                }
            }
        }

        if (direction == 'h') x++;
        if (direction == 'v') y++;
    }

    return true;
}

void Board::placeShip(int x, int y, char directionChar, int size) {
    Direction direction = (directionChar == 'h') ? Direction::HORIZONTAL : Direction::VERTICAL;

    Ship ship({ x, y }, size, direction);
    if (!addShip(ship)) {
        std::cerr << "Error: Failed to add ship to the board." << std::endl;
    }
}

void Board::markSurroundingAsMiss(const Ship& ship) {
    int x = ship.getStartCoordinate().first;
    int y = ship.getStartCoordinate().second;
    int len = ship.getLength();

    int startRow = std::max(0, y - 1);
    int endRow = (ship.getDirection() == Direction::HORIZONTAL) ? y + 1 : y + len;
    int startCol = std::max(0, x - 1);
    int endCol = (ship.getDirection() == Direction::HORIZONTAL) ? x + len : x + 1;

    for (int i = startRow; i <= endRow && i < BOARD_SIZE; i++) {
        for (int j = startCol; j <= endCol && j < BOARD_SIZE; j++) {
            if (grid[i][j] == 'X') {
                grid[i][j] = '*';  // Замена ячейки корабля символом '*'
            }
            else if (grid[i][j] == ' ') {
                grid[i][j] = '.';  // Отметьте ячейки вокруг корабля как промахи
            }
        }
    }
}
