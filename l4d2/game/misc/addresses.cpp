#include "../../framework.h"
#include "../game.h"

void Game::Addresses::Setup()
{
    Paint = Modules::Engine->FindPattern( "55 8B EC 83 EC 54 A1 ? ? ? ? 33 C5 89 45 FC A1" );

    StartDrawing = Modules::MatSurface->FindPattern( "E8 ? ? ? ? 8B 4D 08 8B 06", true );
    FinishDrawing = Modules::MatSurface->FindPattern( "E8 ? ? ? ? 8D 65 E8", true );
    IsPlayerDead = Modules::Client->FindPattern("33 C0 80 B9");
}
