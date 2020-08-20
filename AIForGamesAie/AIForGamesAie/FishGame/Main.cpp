#include <iostream>
#include "raylib.h"
#include "Application.h"
#include "Game.h"

int main()
{
    //Application app(320,180,4);
    //app.Run();

    {
        Game game(320,180,4);
        game.Run();
    }

    return 0;
}
