#include "Board.h"
#include "ComputerPlayer.h"
#include <iostream>

ComputerPlayer::ComputerPlayer() {}

ComputerPlayer::~ComputerPlayer() {}

void ComputerPlayer::setShootingMode(Mode mode) {
    shootingMode = mode;
}

std::pair<int, int> ComputerPlayer::chooseShootCoordinate() {

    //DEBUG
    //std::cout << "�������� ���������� ��� ��������. lastHit: (" << lastHit.first << ", " << lastHit.second << "), shipDirectionDetermined: " << shipDirectionDetermined << std::endl;

    if (shootingMode == Mode::INTELLIGENT) {
        if (initialHit.first == -1 && initialHit.second == -1) {
            // �������� ��������, ���� ��� ������� ���������
            return randomShoot();
        }
        if (!shipDirectionDetermined) {
            // ���� ����������� ������� �� ����������
            return shootAroundInitialHit();
        }
        else {
            // ���� ����������� ������� ��������
            return shootInDirection();
        }
    }
    else {
        // ��������� ����� ��������
        return randomShoot();
    }
}

std::pair<int, int> ComputerPlayer::randomShoot() {
    int x, y;
    do {
        x = rand() % 10;
        y = rand() % 10;
    } while (opponentBoard.isCellShot(x, y));

    std::cout << "\n[������] ��������� ������� � ���������� (" << x << ", " << y << ").\n";
    return std::make_pair(x, y);
}

std::pair<int, int> ComputerPlayer::shootAroundInitialHit() {
    const int dx[] = { -1,  0,  1,  0 };
    const int dy[] = { 0, -1,  0,  1 };

    std::vector<int> directions = { 0, 1, 2, 3 };

    // ������� ����������� ���� ��� ������ �������
    for (int dir = directions.size() - 1; dir >= 0; --dir) {
        int testX = initialHit.first + dx[dir];
        int testY = initialHit.second + dy[dir];

        // �������� ���� �� ������� ��� ��������������
        if (testX < 0 || testX >= Board::getBoardSize() || testY < 0 || testY >= Board::getBoardSize() || opponentBoard.isCellShot(testX, testY)) {
            directions.erase(directions.begin() + dir);
        }
    }

    // ���� ����������� ��������, �� �������� ��������� �� ��� ��� ��������
    if (!directions.empty()) {
        // �������� ��������� ����������� ��� ��������
        int randomIndex = rand() % directions.size();
        int dir = directions[randomIndex];
        int newX = initialHit.first + dx[dir];
        int newY = initialHit.second + dy[dir];
        lastShotDirectionAttempted = (dx[dir] == 0) ? 'v' : 'h';
        std::cout << "\n[����� �����������] ������� � ���������� (" << newX << ", " << newY << ").\n";
        return std::make_pair(newX, newY);
    }

    // ���� ��� ����������� ���� �����������, �� ������ �������.
    //DEBUG
    std::cout << "[����� �����������] ��� ����������� ��� ������, �������� ��������.\n";
    return randomShoot();
}

std::pair<int, int> ComputerPlayer::shootInDirection() {
    int dx = 0, dy = 0;

    // ���������� ����������� �������� �� ������ ���������� ��������� �������� � ���������� ���������
    if (lastShotDirection == 'h') {
        dx = (lastHit.first - initialHit.first > 0) ? 1 : -1; // ���� ��������� ������� ��� ������ ����������, ���� ������, ����� - �����
    }
    else if (lastShotDirection == 'v') {
        dy = (lastHit.second - initialHit.second > 0) ? 1 : -1; // ���� ��������� ������� ��� ���� ����������, ���� ����, ����� - �����
    }

    // ������������ �� �������� �����������, ���� ��������� ���� �������
    if (!isValidShot(lastHit.first + dx, lastHit.second + dy) && !reversedDirection) {
        dx = -dx; // ������ ����������� �� �������� �� �����������
        dy = -dy; // ������ ����������� �� �������� �� ���������
        reversedDirection = true; // ��������, ��� ����������� ���� �������� �� ��������
        // ���������� ��������� ��������� �� ����������, ����� ������ �������� � �������� �����������
        lastHit = initialHit;
    }

    // ��������� ���������� ���������� ��������
    int newX = lastHit.first + dx;
    int newY = lastHit.second + dy;

    if (isValidShot(newX, newY)) {
        // ���� ��������� ������� �������, �������� �� ���� �����������
        return std::make_pair(newX, newY);
    }
    else {
        // ���� ��������� ����������� ��� �������� �� ����������, ���������� ��������� � �������� ��������
        return resetStrategyAndRandomShoot();
    }
}

bool ComputerPlayer::isValidShot(int x, int y) {
    return x >= 0 && x < Board::getBoardSize() && y >= 0 && y < Board::getBoardSize() && !opponentBoard.isCellShot(x, y);
}

//DEBUG
/*std::pair<int, int> ComputerPlayer::shootFromInitialHitInReversedDirection() {
    std::cout << "[��������� ������� shootFromInitialHitInReversedDirection].\n";
    // ��������� ����� ���� � �������� ����������� �� ���������� ���������
    int dx = lastShotDirection == 'h' ? -1 : 0;
    int dy = lastShotDirection == 'v' ? -1 : 0;
    int newX = initialHit.first + dx;
    int newY = initialHit.second + dy;

    // ���������, ��� ����� ���� �������
    if (isValidShot(newX, newY)) {
        std::cout << "[���������� ����� �������� ���� - " << newX << "," << newY << "]" << std::endl;
        return std::make_pair(newX, newY); // ���������� ����� �������� ����
    } else {
        std::cout << "[!] [�������� ����������� ��������� " << newX << "," << newY << "; ���������� ��������� � �������� ��������]" << std::endl;
        // ���� �������� ����������� ���������, ���������� ��������� � �������� ��������
        return resetStrategyAndRandomShoot();
    }
}
*/

std::pair<int, int> ComputerPlayer::resetStrategyAndRandomShoot() {
    //DEBUG
    std::cout << "[����� ���������] ����� ��������� �������� � ������� � ���������� ��������.\n";
    // ���������� ���������
    resetShootingStrategy();
    // ���������� ��������� ���������� ��� ��������
    return randomShoot();
}

void ComputerPlayer::placeShips() {
    // ��������� ���������� ��������
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
    lastHit = { -1, -1 };
    initialHit = { -1, -1 }; // ����� ������� ���������
    shipDirectionDetermined = false;
    reversedDirection = false; // ����� ����� ��������� �����������
    consecutiveHits = 0; // ����� �������� ���������������� ���������
    intelligentTargets.clear();
}

void ComputerPlayer::registerHit(int x, int y) {
    lastHit = { x, y }; // ��������� ��������� ���������
    //DEBUG
    std::cout << "[���������] ���������������� ��������� �: (" << x << ", " << y << ")." << std::endl;

    if (initialHit.first == -1 && initialHit.second == -1) {
        initialHit = lastHit; // ������������� ��������� ���������, ���� ��� ������ ���������
        //DEBUG
        std::cout << "[���������] ��������� ��������� ����������� �: (" << x << ", " << y << ")." << std::endl;
    }
    else {
        // ���������� ����������� ������� ������ ����� ������� ��������� �� ����� � ��������� ����������
        if (!shipDirectionDetermined && (x == initialHit.first || y == initialHit.second)) {
            shipDirectionDetermined = true;
            lastShotDirection = (x == initialHit.first) ? 'v' : 'h';
            //DEBUG
            std::cout << "[���������] ���������� ����������� �������: " << (lastShotDirection == 'v' ? "������������" : "��������������") << std::endl;
        }
    }

    if (shipDirectionDetermined) {
        lastHit = { x, y }; // ��������� ��������� ��������� ��� ����������� �������� ����� �������
        consecutiveHits++; // ����������� ������� ���������������� ���������
        std::cout << "[���������] ���������� ���������������� ��������� ���������: " << consecutiveHits << std::endl;
    }
}

void ComputerPlayer::registerMiss() {
    //DEBUG
     std::cout << "[������] ��������������� ������. ������� ��������� - ����������� ������� ����������: " << (shipDirectionDetermined ? "��" : "���")
         << ", �������� �����������: " << (reversedDirection ? "��" : "���") << std::endl;

    // ���� ����������� ���� ���������� � �� ��� �� �������� � �������� �����������, �������� ��� ��������
    if (shipDirectionDetermined && !reversedDirection) {
        reversedDirection = true; // �������� ������������� �������� � �������� �����������
        lastHit = initialHit; // ������������ � ���������� ���������, ����� ���������� �������� � ������ �����������
        
        //DEBUG 
        std::cout << "[������] ��������� ����������� �������� � ���������� ���������." << std::endl;
    }
    else if (shipDirectionDetermined && reversedDirection) {
        // ���� ����������� ���� ���������� � �� ��� �������� � �������� �����������, �� ��� ��� ������, ���������� ���������
        resetShootingStrategy(); // ����� ��������� ��������

        //DEBUG
        std::cout << "[������] ��������� �������� �������� ����� ��������� ����������� � �������." << std::endl;
    }
    // ���� ����������� �� ���� ����������, ���������� �������� ��������� �������
}

void ComputerPlayer::resetInitialHit() {
    initialHit = { -1, -1 };
}