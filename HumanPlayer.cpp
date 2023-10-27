#include "Board.h"
#include "HumanPlayer.h"

HumanPlayer::HumanPlayer() {
    // �������������, ���� ����������
}

HumanPlayer::~HumanPlayer() {
    // ������������ ��������, ���� ����������
}

std::pair<int, int> HumanPlayer::chooseShootCoordinate() {
    int x, y;
    do {
        std::cout << "������� ���������� X ��� �������� (0-9): ";
        std::cin >> x;
        std::cout << "������� ���������� Y ��� �������� (0-9): ";
        std::cin >> y;
        if (opponentBoard.isCellShot(x, y)) {
            std::cout << "�� ��� �������� � ��� ���������� ��� ��� ������ �������� ��� ������. ���������� ��� ���.\n";
        }
    } while (opponentBoard.isCellShot(x, y));

    return std::make_pair(x, y);
}

void HumanPlayer::placeShips() {
    std::cout << "���������� �������� �� �����:\n";
    // ���������� �������� � ������������ � �� ����������� � ��������
    placeShip(4, 1, "��������������");
    placeShip(3, 2, "�����������");
    placeShip(2, 3, "������������");
    placeShip(1, 4, "������������");
}

void Player::placeShip(int size, int count, const std::string& description) {
    for (int i = 0; i < count; ++i) {
        bool isPlaced = false;
        while (!isPlaced) {
            std::cout << "������� ����������� �������� �� �����:\n";
            ownBoard.display();  // ������� ������� ��������� �����
            std::cout << "\n";

            std::cout << "���������� " << description << " ������� (" << (i + 1) << " �� " << count << "):\n";

            int x, y;
            char direction;

            std::cout << "������� ��������� ���������� X (0-9): ";
            std::cin >> x;
            std::cout << "������� ��������� ���������� Y (0-9): ";
            std::cin >> y;
            std::cout << "������� ����������� ������� (h - ��������������, v - ������������): ";
            std::cin >> direction;

            // ����������� ownBoard ��� ������ ������� canPlaceShip � placeShip
            if (ownBoard.canPlaceShip(x, y, direction, size)) {
                ownBoard.placeShip(x, y, direction, size);
                isPlaced = true;
            }
            else {
                std::cout << "�� ������� ���������� ������� � ��������� �����������. ���������� ��� ���.\n";
            }
        }
    }
}