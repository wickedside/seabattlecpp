#include "Board.h"
#include "HumanPlayer.h"

HumanPlayer::HumanPlayer() {
    // �������������, ���� ����������
}

HumanPlayer::~HumanPlayer() {
    // ������������ ��������, ���� ����������
}

std::pair<int, int> HumanPlayer::chooseShootCoordinate() {
    int x = -1, y = -1;
    do {
        // ������ ���������� X
        while (true) {
            std::cout << "\n[?] ������� ���������� X (0-9) ��� ��������: ";
            char ch = _getch(); // ��������� ������ ��� echo
            if (isdigit(ch) && ch >= '0' && ch <= '9') { // ��������, ��� ��� ����� �� 0 �� 9
                x = ch - '0'; // �������������� char � int
                std::cout << x << std::endl; // ������� ��������� ����� ��� ������������� ������������
                break; // ������� �� �����, ���� ���� ���������
            }
        }

        // ������ ���������� Y
        while (true) {
            std::cout << "\n[?] ������� ���������� Y (0-9) ��� ��������: ";
            char ch = _getch(); // ��������� ������ ��� echo
            if (isdigit(ch) && ch >= '0' && ch <= '9') { // ��������, ��� ��� ����� �� 0 �� 9
                y = ch - '0'; // �������������� char � int
                std::cout << y << std::endl; // ������� ��������� ����� ��� ������������� ������������
                break; // ������� �� �����, ���� ���� ���������
            }
        }
        if (opponentBoard.isCellShot(x, y)) {
            std::cout << "\n[!] �� ��� �������� � ��� ���������� ��� ��� ������ �������� ��� ������. ���������� ��� ���.\n";
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
            std::cout << "\n -> ������� ����������� �������� �� �����:\n";
            ownBoard.display();  // ������� ������� ��������� �����
            std::cout << "\n";

            std::cout << "\n -> ���������� " << description << " ������� (" << (i + 1) << " �� " << count << "):\n";

            int x = -1, y = -1;
            char direction = '\0';

            // ������ ���������� X
            while (true) {
                std::cout << "\n[?] ������� ��������� ���������� X (0-9): ";
                char ch = _getch(); // ��������� ������ ��� echo
                if (isdigit(ch) && ch >= '0' && ch <= '9') { // ��������, ��� ��� ����� �� 0 �� 9
                    x = ch - '0'; // �������������� char � int
                    std::cout << x << std::endl; // ������� ��������� ����� ��� ������������� ������������
                    break; // ������� �� �����, ���� ���� ���������
                }
            }

            // ������ ���������� Y
            while (true) {
                std::cout << "\n[?] ������� ��������� ���������� Y (0-9): ";
                char ch = _getch(); // ��������� ������ ��� echo
                if (isdigit(ch) && ch >= '0' && ch <= '9') { // ��������, ��� ��� ����� �� 0 �� 9
                    y = ch - '0'; // �������������� char � int
                    std::cout << y << std::endl; // ������� ��������� ����� ��� ������������� ������������
                    break; // ������� �� �����, ���� ���� ���������
                }
            }

            // ������ ���������� �������
            while (true) {
                std::cout << "\n[?] ������� ����������� ������� (h - ��������������, v - ������������): ";
                char ch = _getch(); // ��������� ������ ��� echo
                ch = tolower(ch); // �������� � ������� �������� ��� ����������
                if (ch == 'h' || ch == 'v') { // ��������, ��� ���� ��� 'h' ��� 'v'
                    direction = ch;
                    std::cout << direction << std::endl; // ������� ��������� ����������� ��� ������������� ������������
                    break; // ������� �� �����, ���� ���� ���������
                }
            }

            // ����������� ownBoard ��� ������ ������� canPlaceShip � placeShip
            if (ownBoard.canPlaceShip(x, y, direction, size)) {
                ownBoard.placeShip(x, y, direction, size);
                isPlaced = true;
            }
            else {
                std::cout << "[!] �� ������� ���������� ������� � ��������� �����������. ���������� ��� ���.\n";
            }
        }
    }
}

void HumanPlayer::autoPlaceShips() {
    autoPlaceShipsBasic();
}