#ifndef GAME_H
#define GAME_H

#include "HumanPlayer.h"
#include "ComputerPlayer.h"

class Game {
private:
    HumanPlayer human;
    ComputerPlayer computer;
    Player* currentPlayer;
    Player* opponent;

    void switchPlayers();
    void announceWinner();

public:
    Game();
    void start();
};

#endif // GAME_H
