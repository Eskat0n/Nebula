//-------------------------------------------------------------

#include "GameCoreClass_GameStates.h"
#include "GameCoreClass_Game.h"

//-------------------------------------------------------------

State_LevelStarting::State_LevelStarting( Game* const game )
	: _fDuration( MISSION_INTERVAL )
{
	game->GetMsg()->Show();
}

IGameState* State_LevelStarting::Process( Game* const game, float fTimeDelta )
{
	if ( _fDuration <= 0 )
		return new State_InGame( game );
	// Input processing block to allow user escape to menu
	if ( game->CheckControl() ) return 0;

	_fDuration -= fTimeDelta;
	return 0;
}

//-------------------------------------------------------------

State_InGame::State_InGame( Game* const game )
{
	game->GetMsg()->Hide();
	game->GetPlayer( 0 )->Unlock();
	if ( game->GetType() != SINGLEPLAY )
		game->GetPlayer( 1 )->Unlock();
	game->GetArmade()->Unlock();
}

IGameState* State_InGame::Process( Game* const game, float fTimeDelta )
{
	// Input processing block
	if ( game->DispatchControls() ) return 0;

	// Performing physics processing
	game->GetWorld()->ProcessAll( fTimeDelta );
	game->GetArmade()->Process( fTimeDelta );

	// Requesting armade status
	if ( game->GetArmade()->GetShipsCount() == 0 )
	{
		if ( game->GetCurLevel() == 10 )
			return new State_GameComplete( game );
		return new State_CompletingLevel( game );
	}

	// Allow player to process status
	game->GetPlayer( 0 )->Process( fTimeDelta );
	if ( game->GetType() != SINGLEPLAY )
		game->GetPlayer( 1 )->Process( fTimeDelta );

	if ( game->GetPlayer( 0 )->GetState() == Looser || 
		( game->GetType() == SINGLEPLAY && game->GetPlayer( 0 )->GetState() == Looser 
		  && game->GetPlayer( 1 )->GetState() == Looser ) )
		return new State_GameOver( game );

	return 0;
}

//-------------------------------------------------------------

State_CompletingLevel::State_CompletingLevel( Game* const game )
	: _fDuration( COMPLETE_INTERVAL )
{
	game->GetMsg()->SetText( COMPLETE );
	game->GetMsg()->Show();
	game->GetArmade()->Lock();
	game->GetPlayer( 0 )->Lock();
	if ( game->GetType() != SINGLEPLAY )
		game->GetPlayer( 1 )->Lock();
}

IGameState* State_CompletingLevel::Process( Game* const game, float fTimeDelta )
{
	if ( _fDuration <= 0 )
	{
		game->ChangeLevel();
		return new State_LevelStarting( game );
	}
	// Process world phys for preventing bullet and explosives jerks
	game->GetWorld()->ProcessAll( fTimeDelta );

  	_fDuration -= fTimeDelta;
	return 0;
}


//-------------------------------------------------------------

State_GameComplete::State_GameComplete( Game* const game )
	: _fDuration( GAME_COMPLETE )
{
	game->GetMsg()->SetText( GAMECOMPLETE );
	game->GetMsg()->SetPos( 125, 700 );
	game->GetMsg()->Show();

	game->GetArmade()->Lock();
	game->GetPlayer( 0 )->Lock();
	game->GetPlayer( 0 )->SaveStat();
	if ( game->GetType() != SINGLEPLAY )
	{
		game->GetPlayer( 1 )->Lock();
		game->GetPlayer( 1 )->SaveStat();
	}
}

IGameState* State_GameComplete::Process( Game* const game, float fTimeDelta )
{
	if ( _fDuration <= 0 )
		return new State_Credits( game );

	// Input processing block to allow user escape to menu
	if ( game->CheckControl() ) return 0;
	// Process world phys for preventing bullet and explosives jerks
	game->GetWorld()->ProcessAll( fTimeDelta );

  	_fDuration -= fTimeDelta;
	return 0;
}

//-------------------------------------------------------------

State_GameOver::State_GameOver( Game* const game )
	: _fDuration( GAME_OVER )
{
	game->GetMsg()->SetText( GAMEOVER );	
	game->GetMsg()->Show();
}

IGameState* State_GameOver::Process( Game* const game, float fTimeDelta )
{
	if ( _fDuration <= 0 )
	{
		game->End();
		return 0;
	}
  	_fDuration -= fTimeDelta;
	return 0;
}

//-------------------------------------------------------------

State_Credits::State_Credits( Game* const game )
	: _fDuration( CREDITS_TIME )
{
	game->GetMsg()->SetText( CREDITS );
	game->GetMsg()->SetFont( L"Arial", 22 );
	game->GetMsg()->SetFontColor( 0xff00bbff );
	game->GetMsg()->SetPos( 100, 500 );
}

IGameState* State_Credits::Process( Game* const game, float fTimeDelta )
{
	if ( _fDuration <= 0 )
	{
		game->End();
		return 0;
	}
	// Input processing block to allow user escape to menu
	if ( game->CheckControl() ) return 0;

	_fDuration -= fTimeDelta;
	return 0;
}

//-------------------------------------------------------------
