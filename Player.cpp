#include "Player.h"
#include <ctime>

ShipStatus Player::shootAtPlayer(Player& opponent, const std::pair<int, int>& coordinate) {
    ShipStatus status = opponent.getOwnBoard().shootAt(coordinate);
    opponentBoard.shootAt(coordinate); // Добавляем результат выстрела на доску игрока
    return status;
}

bool Player::allShipsSunk() const {
    for (const Ship& ship : ownBoard.getShips()) {
        if (ship.getStatus() != ShipStatus::SUNK) {
            return false;
        }
    }
    return true;
}

void Player::updateOpponentBoard(const Board& board) {
    opponentBoard = board;
}

void Player::autoPlaceShipsBasic() {
    static int seedIncrementer = 37;
    srand(time(0) + seedIncrementer++); // Увеличиваем сид, чтобы у каждого компьютера было уникальное размещение

    // Массив возможных направлений корабля
    const char directions[] = { 'h', 'v' };

    for (int size = 4; size >= 1; --size) {
        for (int count = 1; count <= 5 - size; ++count) {
            bool placed = false;
            while (!placed) {
                int x = rand() % 10;
                int y = rand() % 10;
                char direction = directions[rand() % 2];

                if (ownBoard.canPlaceShip(x, y, direction, size)) {
                    ownBoard.placeShip(x, y, direction, size);
                    placed = true;
                }
            }
        }
    }
}