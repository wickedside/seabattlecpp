#pragma once

#include "HumanPlayer.h"
#include "ComputerPlayer.h"

enum class GameMode {
    HUMAN_VS_COMPUTER,
    COMPUTER_VS_COMPUTER
};

enum class PlacementMode {
    MANUAL,
    AUTO
};

enum class ShootingMode {
    RANDOM,
    INTELLIGENT
};

class Game {
private:
    HumanPlayer human;
    ComputerPlayer computer1;
    ComputerPlayer computer2;
    Player* currentPlayer;
    Player* opponent;

    GameMode gameMode;
    PlacementMode placementMode;
    ShootingMode shootingMode;

    void switchPlayers();
    void announceWinner();

public:
    Game();
    void start();

    void setGameMode(GameMode mode) { gameMode = mode; }
    void setPlacementMode(PlacementMode mode) { placementMode = mode; }
    void setShootingMode(ShootingMode mode) { shootingMode = mode; }
    void humanVsComputerLoop();
    void computerVsComputerLoop();
    void stopGame();
    void pauseGame();
    void checkForPauseOrStop();
    void returnToMainMenu();
};