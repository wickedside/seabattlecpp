#include "Board.h"
#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer() {}

ComputerPlayer::~ComputerPlayer() {}

void ComputerPlayer::setShootingMode(Mode mode) {
    shootingMode = mode;
}

std::pair<int, int> ComputerPlayer::chooseShootCoordinate() {
    if (shootingMode == Mode::INTELLIGENT) {
        if (lastHit.first == -1 && lastHit.second == -1) {
            // �������� ��������, ���� ��� ���������� ���������
            return randomShoot();
        }
        if (!shipDirectionDetermined) {
            // ���� ����������� ������� �� ����������
            return shootAroundLastHit();
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

    return std::make_pair(x, y);
}

std::pair<int, int> ComputerPlayer::shootAroundLastHit() {
    const int dx[] = { -1,  0,  1,  0 };
    const int dy[] = { 0, -1,  0,  1 };

    for (int i = 0; i < 4; ++i) {
        int newX = lastHit.first + dx[i];
        int newY = lastHit.second + dy[i];

        if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10 && !opponentBoard.isCellShot(newX, newY)) {
            return std::make_pair(newX, newY);
        }
    }

    // ���� ��� �������� ������ ���� ��� ���������, ��������� � ��������� ��������
    return randomShoot();
}

std::pair<int, int> ComputerPlayer::shootInDirection() {
    int dx = 0, dy = 0;

    if (lastHitDirection == 'h') {
        dx = (consecutiveHits % 2 == 0) ? 1 : -1;
    }
    else if (lastHitDirection == 'v') {
        dy = (consecutiveHits % 2 == 0) ? 1 : -1;
    }

    int newX = lastHit.first + dx;
    int newY = lastHit.second + dy;

    // ���� ����� ���������� � �������� ����� � ������ �� ���� ���������, ������� ��
    if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10 && !opponentBoard.isCellShot(newX, newY)) {
        return std::make_pair(newX, newY);
    }
    else {
        // �����, ��������� � ��������� ��������
        return randomShoot();
    }
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
    shootingMode = Mode::RANDOM;
    lastHit = { -1, -1 };
    shipDirectionDetermined = false;
    intelligentTargets.clear();
}