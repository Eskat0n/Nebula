//-------------------------------------------------------------
// Nebula Game Core Armade class methods implementation file
//-------------------------------------------------------------

#include "GameCoreClass_Armade.h"

//-------------------------------------------------------------

Armade::Armade( bool boss )
{
	// Nulifies pointers to ships
	for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 10; j++ )
			_pShips[i][j] = 0;

	// Creating control shuip
	_pControlShip = new Pod( new SbrSprite( POD_TEX, 48, 48 ), 0, -20 );
	_pControlShip->Hide();

	if ( boss )
		_pShips[0][0] = new BossShip;
	else
	{
		// Alogrythm written down here is really horrible ^^
		for ( int i = 0; i < 4; i++ )
		{
			for ( int j = 0; j < 10; j++ )
			{
				switch ( i )
				{
					case 0:
						if ( j == 1 || j == 2 || j == 7 || j == 8 )
						{
							_pShips[i][j] = new Tres( new SbrSprite( TRES_TEX, 48, 48 ), j, i );
							break;
						}
						if ( j == 3 || j == 4 || j == 5 || j == 6 )
							_pShips[i][j] = new Gungir( new SbrSprite( GUNGIR_TEX, 48, 48 ), j, i );
						else
							_pShips[i][j] = new Pod( new SbrSprite( POD_TEX, 48, 48 ), j, i );
						break;

					case 1:
						if ( j == 2 || j == 7 )
						{
							_pShips[i][j] = new Borg( new SbrSprite( BORG_TEX, 48, 48 ), j, i );
							break;
						}
						if ( j == 4 || j == 5 )
							_pShips[i][j] = new Tres( new SbrSprite( TRES_TEX, 48, 48 ), j, i );
						else
							_pShips[i][j] = new Pod( new SbrSprite( POD_TEX, 48, 48 ), j, i );
						break;

					case 2:
						if ( j == 1 || j == 3 || j== 6 || j == 8 )
						{ 
							_pShips[i][j] = new Borg( new SbrSprite( BORG_TEX, 48, 48 ), j, i );
							break;
						}

					case 3:	
						_pShips[i][j] = new Pod( new SbrSprite( POD_TEX, 48, 48 ), j, i );
				}
			}
		}
	}

	SbrXRandom::Seed();
	_bMoveLeft = ( SbrXRandom::RandomChance() >= 50 );
	_bMoving = false;

	_bLock = true;
}

//-------------------------------------------------------------

Armade::~Armade()
{
	delete _pControlShip;

	for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 10; j ++ )
			if ( _pShips[i][j] ) 
				delete _pShips[i][j];
}

//-------------------------------------------------------------

void Armade::Process( float fTimeDelta )
{
	if ( _bLock )
		return;

	if ( _bMoving && ( SbrXMath::Around( _pControlShip->GetPosX(), 25, 1 ) 
						|| SbrXMath::Around( _pControlShip->GetPosX(), 325, 1 ) ) )
	{
		_bMoving = false;
		_bMoveLeft = !_bMoveLeft;
	}

	if ( _bMoveLeft && !_bMoving )
	{
		_pControlShip->StrafeLeft();
		for ( int i = 0; i < 4; i++ )
			for ( int j = 0; j < 10; j++ )
				if ( _pShips[i][j] )
					if ( _pShips[i][j]->IsInArmade() )
						_pShips[i][j]->StrafeLeft();
		_bMoving = true;
	}
	if ( !_bMoveLeft && !_bMoving )
	{
		_pControlShip->StrafeRight();
		for ( int i = 0; i < 4; i++ )
			for ( int j = 0; j < 10; j++ )
				if ( _pShips[i][j] )
					if ( _pShips[i][j]->IsInArmade() )
						_pShips[i][j]->StrafeRight();
		_bMoving = true;
	}

	_nShipsCount = 0;
	for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 10; j ++ )
			if ( _pShips[i][j] )
			{
				if ( _pShips[i][j]->GetConsistency() != 0 )
					_nShipsCount++;
				if ( _pShips[i][j]->IsInArmade() ) 
				{
					_pShips[i][j]->Shoot();
					_pShips[i][j]->UseAbility();
				}
			}
}

//-------------------------------------------------------------