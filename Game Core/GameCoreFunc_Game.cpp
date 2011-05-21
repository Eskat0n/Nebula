//-------------------------------------------------------------
//
//-------------------------------------------------------------

#include "GameCoreClass_Game.h"

//-------------------------------------------------------------

Game* Game::_self = 0;
int Game::_nRefCount = 0;

//-------------------------------------------------------------

Game* Game::Instance( GameType type )
{
	if ( !_self )
		_self = new Game( type );

	_nRefCount++;
	return _self;
}

//-------------------------------------------------------------

void Game::Release()
{
	_nRefCount--;
	if ( !_nRefCount )
	{	
		delete _self;
		_self = 0;
	}
}

//-------------------------------------------------------------

Game::Game( GameType type )
	: _pSaber( SaberGE::Instance( DIRECTX ) ),
	  _pAzuEngine( AzuEngine::Instance() ),
	  _pFramer( SbrXFramer::Instance() ),
	  _pWorld( World::Instance() ),
	  _bRunning( true ),
	  _bSuspend( false ),
	  _eType( type )
{
	_pSaber->SetWndTitle( NEBULA_WINDOW );
	_pSaber->PlugFunc( SbrXFramer::MakeEventFromMSG );
	_pSaber->SetOnClose( Game::OnCloseOfGame );
	_pSaber->SetOnFocusGain( Game::Continue );
	_pSaber->SetOnFocusLost( Game::Suspend );
	_pSaber->SetUp();

	// AzuEngine should be already deployed and running
	//_pAzuEngine->SetUp();
	//_pAzuEngine->AddInMemoryWB( EFFECT_BANK );
	//_pAzuEngine->AddStreamingWB( MUSIC_BANK );
	//_pAzuEngine->AddSB( SOUND_BANK );

	// Changing background sound
	_pAzuEngine->Stop( STARTSCREEN_MX );
	_pAzuEngine->Play( INGAME_MX );

	// Loading new sounds
	_pAzuEngine->LoadSound( PLSHOT_MX );
	_pAzuEngine->LoadSound( ROCKET_MX );
	_pAzuEngine->LoadSound( EXPLOSION_MX );
	_pAzuEngine->LoadSound( BOSS_SHOT_MX );
	_pAzuEngine->LoadSound( MEGABLAST_MX );

	if ( type != SINGLEPLAY )
	{
		_pPlayer[0] = new PlayerOne;
		_pPlayer[1] = new PlayerTwo;
	}
	else
		_pPlayer[0] = new Player;

	_pArmade = new Armade();

	// Calibrating message pos
	_pMessage = new SbrHUDMessage();
	_pMessage->SetText( MISSION01 );
	_pMessage->SetPos( 200, 700 );
	_pMessage->SetFont( L"Verdana", 36 );
	_pMessage->SetFontColor( SBR_WHITE );

	_pBackground = new SbrSprite( BKGR01, 800, 800 );

	_pFramer->SetLogicFunc( Game::GameLogic );
	_pFramer->SetRenderFunc( Game::RenderLogic );
}

//-------------------------------------------------------------

Game::~Game()
{
	delete _pState;

	delete _pArmade;
	delete _pPlayer[0];
	if ( _eType != SINGLEPLAY )
		delete _pPlayer[1];
	delete _pMessage;
	delete _pBackground;

	_pWorld->Release();
	_pFramer->Release();

	_pAzuEngine->Stop( INGAME_MX );
	_pAzuEngine->Release();

	_pSaber->Release();
}

//-------------------------------------------------------------

bool Game::CheckControl()
{
	if ( _pFramer->IsKeyDown( SBRK_ESC ) ) 
	{
		End();
		return true;
	}

	return false;
}

bool Game::DispatchControls()
{
	if ( CheckControl() )
		return true;

	if ( _eType == SINGLEPLAY )
	{
		if ( _pFramer->IsKeyDown( SBRK_LEFT ) ) 
			_pPlayer[0]->StrafeLeft();
		if ( _pFramer->IsKeyDown( SBRK_RIGHT ) ) 
			_pPlayer[0]->StrafeRight();

		if ( _pFramer->IsKeyUp( SBRK_LEFT ) ) 
			_pPlayer[0]->Stop();
		if ( _pFramer->IsKeyUp( SBRK_RIGHT ) ) 
			_pPlayer[0]->Stop();

		if ( _pFramer->IsKeyDown( SBRK_X ) ) 
			_pPlayer[0]->Shoot();
		if ( _pFramer->IsKeyDown( SBRK_SHIFT ) )
			_pPlayer[0]->ShootRocket();
	}

	if ( _eType == HOTSEAT )
	{
		// Handling player 1 input
		if ( _pFramer->IsKeyDown( SBRK_LEFT ) ) 
			_pPlayer[0]->StrafeLeft();
		if ( _pFramer->IsKeyDown( SBRK_RIGHT ) ) 
			_pPlayer[0]->StrafeRight();

		if ( _pFramer->IsKeyUp( SBRK_LEFT ) ) 
			_pPlayer[0]->Stop();
		if ( _pFramer->IsKeyUp( SBRK_RIGHT ) ) 
			_pPlayer[0]->Stop();

		if ( _pFramer->IsKeyDown( SBRK_L ) ) 
			_pPlayer[0]->Shoot();
		if ( _pFramer->IsKeyDown( SBRK_K ) )
			_pPlayer[0]->ShootRocket();		

		// Handling player 2 input
		if ( _pFramer->IsKeyDown( SBRK_C ) ) 
			_pPlayer[1]->StrafeLeft();
		if ( _pFramer->IsKeyDown( SBRK_V ) ) 
			_pPlayer[1]->StrafeRight();

		if ( _pFramer->IsKeyUp( SBRK_C ) ) 
			_pPlayer[1]->Stop();
		if ( _pFramer->IsKeyUp( SBRK_V ) ) 
			_pPlayer[1]->Stop();

		if ( _pFramer->IsKeyDown( SBRK_W ) ) 
			_pPlayer[1]->Shoot();
		if ( _pFramer->IsKeyDown( SBRK_Q ) )
			_pPlayer[1]->ShootRocket();	
	}

	return false;
}

//-------------------------------------------------------------

bool Game::ChangeLevel()
{
	_nCurLevel++;

	switch ( _nCurLevel) 
	{
		case 1:
			_pMessage->SetText( MISSION01 ); 
			_pBackground->SetTexture( BKGR01 );
			break;
		case 2:
			_pMessage->SetText( MISSION02 ); 
			_pBackground->SetTexture( BKGR02 );
			break;
		case 3:
			_pMessage->SetText( MISSION03 ); 
			_pBackground->SetTexture( BKGR03 );
			break;
		case 4:
			_pMessage->SetText( MISSION04 ); 
			_pBackground->SetTexture( BKGR04 );
			break;
		case 5:
			_pMessage->SetText( MISSION05 ); 
			_pBackground->SetTexture( BKGR05 );
			break;
		case 6:
			_pMessage->SetText( MISSION06 ); 
			_pBackground->SetTexture( BKGR06 );
			break;
		case 7:
			_pMessage->SetText( MISSION07 ); 
			_pBackground->SetTexture( BKGR07 );
			break;
		case 8:
			_pMessage->SetText( MISSION08 ); 
			_pBackground->SetTexture( BKGR08 );
			break;
		case 9:
			_pMessage->SetText( MISSION09 ); 
			_pBackground->SetTexture( BKGR09 );
			break;
		case 10:
			_pMessage->SetText( MISSION10 ); 
			_pBackground->SetTexture( BKGR10 );
			break;
	}

	// Renew armade
	delete _pArmade;
	_pArmade = new Armade( _nCurLevel == 10 );
	// Reset position of player ships
	_pPlayer[0]->ResetPos();
	if ( _eType != SINGLEPLAY )
		_pPlayer[1]->ResetPos();

	return true;
}

//-------------------------------------------------------------

bool Game::GameLogic()
{
	if ( !_self )
		return true;

	return _self->Process();
}

//-------------------------------------------------------------

bool Game::RenderLogic()
{
	if ( !_self )
		return true;

	_self->_pSaber->Clear( SBR_WHITE );
	_self->_pSaber->BeginScene();
	_self->_pBackground->Draw( 0, 0 );
	_self->_pWorld->RenderAll();
	_self->_pPlayer[0]->RenderCounters();
	if ( _self->_eType != SINGLEPLAY )
		_self->_pPlayer[1]->RenderCounters();
	_self->_pMessage->Draw();
	_self->_pSaber->EndScene();

	return false;
}

//-------------------------------------------------------------

void Game::OnCloseOfGame()
{
	if ( _self )
		_self->End();
}

//-------------------------------------------------------------

void Game::Begin()
{
	_pState = new State_LevelStarting( this );
	_nCurLevel = 0;
	ChangeLevel();

	_pFramer->EnterMessageLoop();
}

//-------------------------------------------------------------

void Game::End()
{
	_pSaber->Halt();
	_bRunning = false;
}

//-------------------------------------------------------------

void Game::Suspend()
{
	if ( _self )
		_self->_bSuspend = true;
}

//-------------------------------------------------------------

void Game::Continue()
{
	if ( _self )
		_self->_bSuspend = false;
}

//-------------------------------------------------------------

bool Game::Process()
{
	if ( !_bSuspend )
		if ( IGameState* newstate = _pState->Process( this, _pFramer->GetDelta() ) )
		{
			delete _pState;
			_pState = newstate;
		}

	return !_bRunning;
}