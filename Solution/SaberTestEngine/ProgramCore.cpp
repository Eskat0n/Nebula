#include <windows.h>
#include <process.h>

#include "c:\Users\Eskat0n\Documents\dev_game\Nebula\code\Game Core\GameCoreClass_Game.h"

//-----------------------------------------------------------------------------

Game* game;

//-----------------------------------------------------------------------------

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd )
{
	game = Game::Instance( SINGLEPLAY );
	game->Begin();
	game->Release();

    return 0;
}
