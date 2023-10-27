#include "Player.h"

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