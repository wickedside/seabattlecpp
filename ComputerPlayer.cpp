#include "Board.h"
#include "ComputerPlayer.h"
#include <ctime>

ComputerPlayer::ComputerPlayer() {}

ComputerPlayer::~ComputerPlayer() {}

void ComputerPlayer::setShootingMode(Mode mode) {
    shootingMode = mode;
}

std::pair<int, int> ComputerPlayer::chooseShootCoordinate() {
    if (shootingMode == Mode::INTELLIGENT) {
        if (!shipDirectionDetermined && !intelligentTargets.empty()) {
            auto target = intelligentTargets.back();
            intelligentTargets.pop_back();
            return target;
        }

        if (shipDirection == 'h') {
            // Попробуйте выстрелить влево или вправо от последнего попадания
            for (int dx = -1; dx <= 1; dx += 2) {
                int x = lastHit.first + dx;
                int y = lastHit.second;

                if (x >= 0 && x < 10) {
                    return std::make_pair(x, y);
                }
            }
        }
        else if (shipDirection == 'v') {
            // Попробуйте выстрелить вверх или вниз от последнего попадания
            for (int dy = -1; dy <= 1; dy += 2) {
                int x = lastHit.first;
                int y = lastHit.second + dy;

                if (y >= 0 && y < 10) {
                    return std::make_pair(x, y);
                }
            }
        }
    }

    // Если мы здесь, значит у нас случайный выстрел или нам не удалось определить интеллектуальную цель
    int x, y;
    do {
        x = rand() % 10;
        y = rand() % 10;
    } while (opponentBoard.isCellShot(x, y));

    return std::make_pair(x, y);
}

void ComputerPlayer::placeShips() {
    // Рандомное размещение кораблей
    generateRandomShips();
}

void ComputerPlayer::generateRandomShips() {
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

void ComputerPlayer::addSurroundingCoordinates(int x, int y) {
    const int dx[] = { -1,  0,  1,  0 };
    const int dy[] = { 0, -1,  0,  1 };

    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10) {
            intelligentTargets.emplace_back(newX, newY);
        }
    }
}

void ComputerPlayer::resetShootingStrategy() {
    shootingMode = Mode::RANDOM;
    lastHit = { -1, -1 };
    shipDirectionDetermined = false;
    intelligentTargets.clear();
}