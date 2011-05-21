//-------------------------------------------------------------
// Nebula Game Core player class methods implementation file
//-------------------------------------------------------------

#include "GameCoreClass_Player.h"
#include "../FileAccess/DataLayer.h"

//-------------------------------------------------------------

Player::Player()
	: _pShip( new PlayerShip() ),
	  _nNextCap( 1000 ),
	  _eState( Alive ),
	  _bLock( true )
{
	// Setting up counters
	_pLives = new SbrHUDCounter();
	_pScore = new SbrHUDCounter();
	_pAmmo = new SbrHUDCounter();

	_pLivesSpr = new SbrSprite( LIFE_TEX, 64, 64 );
	_pAmmoSpr = new SbrSprite( ROCKET_TEX, 16, 16 );

	_pCombo = new Combo( 1 );

	_pScore->SetPos( 20, 30 );
	_pLives->SetPos( 10, 80 );
	_pAmmo->SetPos( 70, 80 );

	_pScore->SetValue( 0 );
	_pScore->SetFont( L"Arial", 24 );
	_pScore->SetFontColor( SBR_RED );
	_pLives->SetValue( 3 );
	_pLives->SetFont( L"Arial", 24 );
	_pLives->SetFontColor( SBR_RED );
	_pAmmo->SetValue( 5 );
	_pAmmo->SetFont( L"Arial", 24 );
	_pAmmo->SetFontColor( SBR_RED );
}

//-------------------------------------------------------------

Player::~Player()
{
	delete _pShip;

	delete _pLives;
	delete _pScore;
	delete _pAmmo;

	delete _pLivesSpr;
	delete _pAmmoSpr;

	delete _pCombo;
}

//-------------------------------------------------------------

void Player::Process( float fTimeDelta )
{
	switch ( _eState )
	{
		case Alive:
			if ( _pShip->GetConsistency() == 0 )
			{
				if ( !_pLives->GetValue() )
				{
					_eState = Looser;
					Lock();
					break;
				}
				_pLives->SetValue( _pLives->GetValue() - 1 );
				_eState = Spawning;
				Lock();
				_fCooldown = PL_SPAWN_TIME;
			}
			break;

		case Spawning:
			if ( _fCooldown <= 0 )
			{
				_pShip->Repair();
				ResetPos();
				_eState = Spawned;
				_fCooldown = PL_IMMORTAL_TIME;
				Shield();
				Unlock();
			}
			_fCooldown -= fTimeDelta;
			break;

		case Spawned:
			if ( _fCooldown <= 0 )
			{
				Unshield();
				_eState = Alive;
			}
			_fCooldown -= fTimeDelta;
			break;

	}

	// Allow combo to update its state
	_pCombo->Process( fTimeDelta );

	// Calculating score profit from last frame
	if ( int i = _pShip->GetCost() )
	{
		_pCombo->Increase();
		int ScorePlus = _pCombo->GetRatio() * i;
		_pScore->SetValue( _pScore->GetValue() + ScorePlus );
	}

	// Checking score for exceeding the score cap
	if ( _pScore->GetValue() >= _nNextCap )
	{
		_pLives->SetValue( _pLives->GetValue() + 1 );
		switch ( _nNextCap )
		{
			case 1000:		_nNextCap = 5000;	break;
			case 5000:		_nNextCap = 10000;	break;
			case 10000:		_nNextCap = 25000;	break;
			case 25000:		_nNextCap = 50000;	break;
			case 50000:		_nNextCap = 100000; break;
			case 100000:	_nNextCap = 250000; break;
			case 250000:	_nNextCap = 500000;
		}
	}
}

//-------------------------------------------------------------

void Player::RenderCounters()
{
	_pLives->Draw();
	_pScore->Draw();
	_pAmmo->Draw();
	_pCombo->Render();

	_pLivesSpr->Draw( 20, 22 );
	_pAmmoSpr->Draw( 85, 31 );
}

//-------------------------------------------------------------

void Player::Shoot()
{
	if ( !_bLock ) 
		_pShip->Shoot();
}

//-------------------------------------------------------------

void Player::ShootRocket()
{
	if ( !_bLock )
		if ( _pAmmo->GetValue() > 0 )
		{
			_pShip->ShootRocket();
			_pAmmo->SetValue( _pShip->GetRockets() );
		}
}

//-------------------------------------------------------------

void Player::ResetPos()
{
	_pShip->SetPos( 400, 550 );
}

//-------------------------------------------------------------

void Player::Shield()
{
	_pShip->Destroyable( false );
	_pShip->ChangeColor( 0x99ffffff );
}

void Player::Unshield()
{
	_pShip->Destroyable();
	_pShip->ChangeColor( SBR_WHITE );
}

//-------------------------------------------------------------

bool Player::SaveStat() const
{
	IDataLayer* pDataLayer = DataLayer::Instance();
	bool b = pDataLayer->WriteStat( L"singleplay", 
									(int)_pLives->GetValue(), (int)_pAmmo->GetValue(), (int)_pScore->GetValue() );
	pDataLayer->Release();

	return b;
}

//-------------------------------------------------------------

PlayerOne::PlayerOne()
	: Player()
{
	_pShip->ChangeColor( 0xffffbbbb );
	ResetPos();
}

//-------------------------------------------------------------

void PlayerOne::ResetPos()
{
	_pShip->SetPos( 350, 550 );
}

//-------------------------------------------------------------

void PlayerOne::Shield()
{
	_pShip->Destroyable( false );
	_pShip->ChangeColor( 0x99ffbbbb );
}

void PlayerOne::Unshield()
{
	_pShip->Destroyable();
	_pShip->ChangeColor( 0xffffbbbb );
}

//-------------------------------------------------------------

bool PlayerOne::SaveStat() const
{
	IDataLayer* pDataLayer = DataLayer::Instance();
	bool b = pDataLayer->WriteStat( L"player01", 
									(int)_pLives->GetValue(), (int)_pAmmo->GetValue(), (int)_pScore->GetValue() );
	pDataLayer->Release();

	return b;
}

PlayerTwo::PlayerTwo()
	: Player()
{
	_pShip->ChangeColor( 0xffbbbbff );
	ResetPos();

	delete _pCombo;
	_pCombo = new Combo( 2 );

	_pScore->SetPos( 710, 30 );
	_pLives->SetPos( 700, 80 );
	_pAmmo->SetPos( 760, 80 );
}

//-------------------------------------------------------------

void PlayerTwo::ResetPos()
{
	_pShip->SetPos( 450, 550 );
}

//-------------------------------------------------------------

void PlayerTwo::Shield()
{
	_pShip->Destroyable( false );
	_pShip->ChangeColor( 0x99bbbbff );
}

void PlayerTwo::Unshield()
{
	_pShip->Destroyable();
	_pShip->ChangeColor( 0xffbbbbff );
}

//-------------------------------------------------------------

void PlayerTwo::RenderCounters()
{
	Player::RenderCounters();
	_pLivesSpr->Draw( 710, 22 );
	_pAmmoSpr->Draw( 775, 31 );
}

bool PlayerTwo::SaveStat() const
{
	IDataLayer* pDataLayer = DataLayer::Instance();
	bool b = pDataLayer->WriteStat( L"player02", 
									(int)_pLives->GetValue(), (int)_pAmmo->GetValue(), (int)_pScore->GetValue() );
	pDataLayer->Release();

	return b;
}