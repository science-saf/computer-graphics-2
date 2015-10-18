#include "gameapplication.h"

int main(int argc, char *argv[])
{
    GameApplication a(argc, argv);
    return a.enterGameLoop();
}
