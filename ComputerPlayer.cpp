#include "Board.h"
#include "ComputerPlayer.h"
#include <iostream>

ComputerPlayer::ComputerPlayer() {}

ComputerPlayer::~ComputerPlayer() {}

void ComputerPlayer::setShootingMode(Mode mode) {
    shootingMode = mode;
}

std::pair<int, int> ComputerPlayer::chooseShootCoordinate() {
    std::cout << "Choosing shoot coordinate. LastHit: (" << lastHit.first << ", " << lastHit.second << "), shipDirectionDetermined: " << shipDirectionDetermined << std::endl;

    if (shootingMode == Mode::INTELLIGENT) {
        if (initialHit.first == -1 && initialHit.second == -1) {
            // —трел€ем рандомно, если нет первого попадани€
            return randomShoot();
        }
        if (!shipDirectionDetermined) {
            // ≈сли направление корабл€ не определено
            return shootAroundInitialHit();
        }
        else {
            // ≈сли направление корабл€ известно
            return shootInDirection();
        }
    }
    else {
        // —лучайный режим стрельбы
        return randomShoot();
    }
}

std::pair<int, int> ComputerPlayer::randomShoot() {
    int x, y;
    do {
        x = rand() % 10;
        y = rand() % 10;
    } while (opponentBoard.isCellShot(x, y));

    return std::make_pair(x, y);
}

std::pair<int, int> ComputerPlayer::shootAroundInitialHit() {
    const int dx[] = { -1,  0,  1,  0 };
    const int dy[] = { 0, -1,  0,  1 };

    for (int i = 0; i < 4; ++i) {
        int newX = initialHit.first + dx[i];
        int newY = initialHit.second + dy[i];

        // ѕропустить направление предыдущего промаха
        if ((dx[i] == 0 && lastShotDirectionAttempted == 'v') ||
            (dy[i] == 0 && lastShotDirectionAttempted == 'h')) {
            continue;
        }

        if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10 && !opponentBoard.isCellShot(newX, newY)) {
            lastShotDirectionAttempted = (dx[i] == 0) ? 'v' : 'h';
            return std::make_pair(newX, newY);
        }
    }

    // ≈сли все соседние клетки были уже атакованы, вернутьс€ к случайной стрельбе
    return randomShoot();
}

std::pair<int, int> ComputerPlayer::shootInDirection() {
    int dx = 0, dy = 0;

    // ќпределение направлени€ последнего успешного выстрела
    if (lastHitDirection == 'h') {
        dx = (successfulShotDirection == 'h' && !reversedDirection) ? 1 : -1;
    }
    else if (lastHitDirection == 'v') {
        dy = (successfulShotDirection == 'v' && !reversedDirection) ? 1 : -1;
    }

    int newX = (reversedDirection) ? initialHit.first + dx : lastHit.first + dx;
    int newY = (reversedDirection) ? initialHit.second + dy : lastHit.second + dy;

    if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10 && !opponentBoard.isCellShot(newX, newY)) {
        return std::make_pair(newX, newY);
    }
    else {
        if (!reversedDirection) {
            reversedDirection = true;
            return shootInDirection(); // ѕробуем стрел€ть в обратное направление
        }
        else {
            reversedDirection = false; // сбросить флаг
            resetShootingStrategy();   // ≈сли стрел€ли в обратное направление и промахнулись, сбросим стратегию
            return randomShoot();
        }
    }
}

void ComputerPlayer::placeShips() {
    // –андомное размещение кораблей
    autoPlaceShips();
}

void ComputerPlayer::autoPlaceShips() {
    autoPlaceShipsBasic();
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
    initialHit = { -1, -1 }; // —брос первого попадани€
    shipDirectionDetermined = false;
    reversedDirection = false; // ƒобавлено
    intelligentTargets.clear();
}

void ComputerPlayer::registerHit(int x, int y) {
    if (initialHit.first == -1 && initialHit.second == -1) {
        // Ёто первое попадание по кораблю
        initialHit = { x, y };
        lastHit = { x, y };
        shipDirectionDetermined = false;
    }
    else {
        // Ёто последующее попадание по кораблю
        lastHit = { x, y };
        if (x == initialHit.first) {
            lastHitDirection = 'v'; // вертикальное направление
            successfulShotDirection = 'v'; // обновить направление последнего успешного выстрела
        }
        else {
            lastHitDirection = 'h'; // горизонтальное направление
            successfulShotDirection = 'h'; // обновить направление последнего успешного выстрела
        }
        shipDirectionDetermined = true;
    }
}

void ComputerPlayer::registerMiss() {
    if (shipDirectionDetermined && !reversedDirection) {
        reversedDirection = true; // ѕопробуем стрел€ть в обратное направление на следующем ходу
    }
    else if (reversedDirection) {
        resetShootingStrategy(); // ≈сли мы уже пробовали обратное направление и промахнулись, сбросим стратегию
    }
}

void ComputerPlayer::resetInitialHit() {
    initialHit = { -1, -1 };
}

std::pair<int, int> ComputerPlayer::getLastHit() const {
    return lastHit;
}