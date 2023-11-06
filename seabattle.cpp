#include "Game.h"
#include "mainmenu.h"

int main() {
    setlocale(LC_ALL, "");
    Game seaBattle;
    MainMenu menu;
    menu.handleMenuSelection(seaBattle);
    return 0;
}