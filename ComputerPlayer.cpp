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
    //std::cout << "Выбираем координату для выстрела. lastHit: (" << lastHit.first << ", " << lastHit.second << "), shipDirectionDetermined: " << shipDirectionDetermined << std::endl;

    if (shootingMode == Mode::INTELLIGENT) {
        if (initialHit.first == -1 && initialHit.second == -1) {
            // Стреляем рандомно, если нет первого попадания
            return randomShoot();
        }
        if (!shipDirectionDetermined) {
            // Если направление корабля не определено
            return shootAroundInitialHit();
        }
        else {
            // Если направление корабля известно
            return shootInDirection();
        }
    }
    else {
        // Случайный режим стрельбы
        return randomShoot();
    }
}

std::pair<int, int> ComputerPlayer::randomShoot() {
    int x, y;
    do {
        x = rand() % 10;
        y = rand() % 10;
    } while (opponentBoard.isCellShot(x, y));

    std::cout << "\n[РАНДОМ] Случайный выстрел в координату (" << x << ", " << y << ").\n";
    return std::make_pair(x, y);
}

std::pair<int, int> ComputerPlayer::shootAroundInitialHit() {
    const int dx[] = { -1,  0,  1,  0 };
    const int dy[] = { 0, -1,  0,  1 };

    std::vector<int> directions = { 0, 1, 2, 3 };

    // Убираем направления куда был сделан выстрел
    for (int dir = directions.size() - 1; dir >= 0; --dir) {
        int testX = initialHit.first + dx[dir];
        int testY = initialHit.second + dy[dir];

        // Проверка поля на выстрел или некорректность
        if (testX < 0 || testX >= Board::getBoardSize() || testY < 0 || testY >= Board::getBoardSize() || opponentBoard.isCellShot(testX, testY)) {
            directions.erase(directions.begin() + dir);
        }
    }

    // Если направления остались, то выбираем рандомное из них для выстрела
    if (!directions.empty()) {
        // Выбираем рандомное направления для выстрела
        int randomIndex = rand() % directions.size();
        int dir = directions[randomIndex];
        int newX = initialHit.first + dx[dir];
        int newY = initialHit.second + dy[dir];
        lastShotDirectionAttempted = (dx[dir] == 0) ? 'v' : 'h';
        std::cout << "\n[ПОИСК НАПРАВЛЕНИЯ] Выстрел в координату (" << newX << ", " << newY << ").\n";
        return std::make_pair(newX, newY);
    }

    // Если все направления были испробованы, то рандом выстрел.
    //DEBUG
    std::cout << "[ПОИСК НАПРАВЛЕНИЯ] Все направления уже заняты, стреляем рандомно.\n";
    return randomShoot();
}

std::pair<int, int> ComputerPlayer::shootInDirection() {
    int dx = 0, dy = 0;

    // Определяем направление стрельбы на основе последнего успешного выстрела и начального попадания
    if (lastShotDirection == 'h') {
        dx = (lastHit.first - initialHit.first > 0) ? 1 : -1; // Если последний выстрел был правее начального, идем вправо, иначе - влево
    }
    else if (lastShotDirection == 'v') {
        dy = (lastHit.second - initialHit.second > 0) ? 1 : -1; // Если последний выстрел был ниже начального, идем вниз, иначе - вверх
    }

    // Переключение на обратное направление, если достигнут край корабля
    if (!isValidShot(lastHit.first + dx, lastHit.second + dy) && !reversedDirection) {
        dx = -dx; // Меняем направление на обратное по горизонтали
        dy = -dy; // Меняем направление на обратное по вертикали
        reversedDirection = true; // Помечаем, что направление было изменено на обратное
        // Сбрасываем последнее попадание до начального, чтобы начать стрельбу в обратном направлении
        lastHit = initialHit;
    }

    // Вычисляем координаты следующего выстрела
    int newX = lastHit.first + dx;
    int newY = lastHit.second + dy;

    if (isValidShot(newX, newY)) {
        // Если следующий выстрел валиден, стреляем по этим координатам
        return std::make_pair(newX, newY);
    }
    else {
        // Если валидного направления для стрельбы не существует, сбрасываем стратегию и стреляем случайно
        return resetStrategyAndRandomShoot();
    }
}

bool ComputerPlayer::isValidShot(int x, int y) {
    return x >= 0 && x < Board::getBoardSize() && y >= 0 && y < Board::getBoardSize() && !opponentBoard.isCellShot(x, y);
}

//DEBUG
/*std::pair<int, int> ComputerPlayer::shootFromInitialHitInReversedDirection() {
    std::cout << "[Сработала функция shootFromInitialHitInReversedDirection].\n";
    // Вычисляем новую цель в обратном направлении от начального попадания
    int dx = lastShotDirection == 'h' ? -1 : 0;
    int dy = lastShotDirection == 'v' ? -1 : 0;
    int newX = initialHit.first + dx;
    int newY = initialHit.second + dy;

    // Проверяем, что новая цель валидна
    if (isValidShot(newX, newY)) {
        std::cout << "[Возвращаем новую валидную цель - " << newX << "," << newY << "]" << std::endl;
        return std::make_pair(newX, newY); // Возвращаем новую валидную цель
    } else {
        std::cout << "[!] [Обратное направление невалидно " << newX << "," << newY << "; сбрасываем стратегию и стреляем случайно]" << std::endl;
        // Если обратное направление невалидно, сбрасываем стратегию и стреляем случайно
        return resetStrategyAndRandomShoot();
    }
}
*/

std::pair<int, int> ComputerPlayer::resetStrategyAndRandomShoot() {
    //DEBUG
    std::cout << "[СБРОС СТРАТЕГИИ] Сброс стратегии стрельбы и переход к случайному выстрелу.\n";
    // Сбрасываем стратегию
    resetShootingStrategy();
    // Возвращаем случайную координату для выстрела
    return randomShoot();
}

void ComputerPlayer::placeShips() {
    // Рандомное размещение кораблей
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
    initialHit = { -1, -1 }; // Сброс первого попадания
    shipDirectionDetermined = false;
    reversedDirection = false; // Сброс флага обратного направления
    consecutiveHits = 0; // Сброс подсчета последовательных попаданий
    intelligentTargets.clear();
}

void ComputerPlayer::registerHit(int x, int y) {
    lastHit = { x, y }; // Обновляем последнее попадание
    //DEBUG
    std::cout << "[ПОПАДАНИЕ] Зарегистрировано попадание в: (" << x << ", " << y << ")." << std::endl;

    if (initialHit.first == -1 && initialHit.second == -1) {
        initialHit = lastHit; // Устанавливаем начальное попадание, если это первое попадание
        //DEBUG
        std::cout << "[ПОПАДАНИЕ] Начальное попадание установлено в: (" << x << ", " << y << ")." << std::endl;
    }
    else {
        // Определяем направление корабля только после второго попадания по линии с начальным попаданием
        if (!shipDirectionDetermined && (x == initialHit.first || y == initialHit.second)) {
            shipDirectionDetermined = true;
            lastShotDirection = (x == initialHit.first) ? 'v' : 'h';
            //DEBUG
            std::cout << "[ПОПАДАНИЕ] Определено направление корабля: " << (lastShotDirection == 'v' ? "вертикальное" : "горизонтальное") << std::endl;
        }
    }

    if (shipDirectionDetermined) {
        lastHit = { x, y }; // Обновляем последнее попадание для продолжения стрельбы вдоль корабля
        consecutiveHits++; // Увеличиваем счётчик последовательных попаданий
        std::cout << "[ПОПАДАНИЕ] Количество последовательных попаданий увеличено: " << consecutiveHits << std::endl;
    }
}

void ComputerPlayer::registerMiss() {
    //DEBUG
     std::cout << "[ПРОМАХ] Зарегистрирован промах. Текущее состояние - Направление корабля определено: " << (shipDirectionDetermined ? "да" : "нет")
         << ", Обратное направление: " << (reversedDirection ? "да" : "нет") << std::endl;

    // Если направление было определено и мы еще не стреляли в обратном направлении, пытаемся его изменить
    if (shipDirectionDetermined && !reversedDirection) {
        reversedDirection = true; // Помечаем необходимость стрелять в обратном направлении
        lastHit = initialHit; // Возвращаемся к начальному попаданию, чтобы попытаться стрелять в другом направлении
        
        //DEBUG 
        std::cout << "[ПРОМАХ] Изменение направления стрельбы с начального попадания." << std::endl;
    }
    else if (shipDirectionDetermined && reversedDirection) {
        // Если направление было определено и мы уже стреляли в обратном направлении, но это был промах, сбрасываем стратегию
        resetShootingStrategy(); // Сброс стратегии стрельбы

        //DEBUG
        std::cout << "[ПРОМАХ] Стратегия стрельбы сброшена после изменения направления и промаха." << std::endl;
    }
    // Если направление не было определено, продолжаем стрелять случайным образом
}

void ComputerPlayer::resetInitialHit() {
    initialHit = { -1, -1 };
}