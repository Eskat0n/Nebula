//-------------------------------------------------------------
// Nebula Game Core ships classes methods implementation file
//-------------------------------------------------------------

#include "../Azumaria Sound Engine/AzuEngineClass.h"

#include "GameCoreClass_Ships.h"

//-------------------------------------------------------------
// Shot methods
//-------------------------------------------------------------

Shot::Shot( int id, SbrSprite* spr, int dmg, int speed, Ship* owner )
		: MovingObject( id, 0, 0 )
{
	_bVisible = false;
	_nDmg = dmg;
	Speed = speed;

	_pOwner = owner;

	SetSprite( spr );
	SetHotSpot( 8, 8 );

	SetHitbox( 4, 4, 8, 8 );

	SetMoveType( new MV_TRJ_STRAIGHT( &_ptPos, Speed ) );
}

//-------------------------------------------------------------

void Shot::Process( float fTimeDelta )
{
	MovingObject::Process( fTimeDelta );
}

//-------------------------------------------------------------

void Shot::OnCollide( Object* obj )
{
	switch ( _nID )
	{
		case SHOT_PLAYER_ROCKET:
			if ( obj->GetID() == ENEMY_BOSS )
			{
				Hide();
				Stop();
				SetPos( -200, -200 );
			}
			break;

		case SHOT_PLAYER: 
			if ( ( obj->GetID() == ENEMY01 || obj->GetID() == ENEMY02 || 
				obj->GetID() == ENEMY03 || obj->GetID() == ENEMY04 || obj->GetID() == ENEMY_BOSS ) && IsVisible() )
			{
				Hide();
				Stop();
				SetPos( -200, -200 );
			}
			break;

		case SHOT_ENEMY:
			if ( ( obj->GetID() == PLSHIP01 ) && IsVisible() )
			{
				Hide();
				Stop();
				SetPos( -200, -200 );
			}
	}
}

//-------------------------------------------------------------

void Shot::Shoot( int x, int y, float ang )
{
	SetPos( x, y );
	MoveTo( x + cos(ang) * 1000, y + sin(ang) * 1000 );
	Show();
}

//-------------------------------------------------------------

void Shot::Shoot( int x, int y, int mvx, int mvy )
{
	SetPos( x, y );
	MoveTo( mvx, mvy );
	Show();
}

//-------------------------------------------------------------
// Ship methods
//-------------------------------------------------------------

Ship::Ship( int maxarm )
	: MovingObject( PLSHIP01, 400, 550 ),
	  _nCurArmor( maxarm ),
	  _nMaxArmor( maxarm ),
	  _bDestroyable( true ),
	  _nCost( 0 )
{
	SbrAnim* anim = new SbrAnim( EXPLOSIVE_TEX, 7, 7, 64, 64 );
	anim->SetHotSpot( 32, 32 );
	_pDestroyEffect = new Effect( EXPLOSIVE, anim );

	SetHitbox( 8, 8, 32, 32 );
}

//-------------------------------------------------------------

Ship::~Ship()
{
	if ( _pDestroyEffect )	delete _pDestroyEffect;
}

//-------------------------------------------------------------

void Ship::Destroy()
{
	if ( _pDestroyEffect ) 
		_pDestroyEffect->Perform( _ptPos.x, _ptPos.y );	
	_nCurArmor = 0;
	Hide();
	Stop();
	SetPos( -200, -200 );

	AzuCreate( pAzu );
	pAzu->Play( EXPLOSION_MX );
	pAzu->Release();
}

//-------------------------------------------------------------

bool Ship::Hit( int damage )
{
	if ( _bDestroyable )
	{
		_nCurArmor -= damage;
		if ( _nCurArmor <= 0 )
		{
			Destroy();
			return false;
		}
	}

	return true;
}

//-------------------------------------------------------------
// Player Ship methods
//-------------------------------------------------------------

PlayerShip::PlayerShip()
	: Ship( PLSHIP_ARMOR ),
	  _nCurMainShot( 0 ),
	  _bMainWpnReload( false ),
	  _pRockets( new Shot( SHOT_PLAYER_ROCKET, new SbrSprite( ROCKET_TEX, 16, 16 ), ROCKET_DMG, ROCKET_SPEED, this ) ),
	  _nRocketsCount( 5 ),
	  _bRocketsReload( false )
{
	SetSprite( new SbrSprite( PLSHIP_TEX, 48, 48 ) );

	SetHotSpot( 24, 24 );
	SetHitbox( 20, 20, 8, 8 );
	
	_pMainWpn[0] = new Shot( SHOT_PLAYER, new SbrSprite( PLSHOT_TEX, 24, 24 ), BLASTER_DMG, BLASTER_SPEED, this );
	_pMainWpn[1] = new Shot( SHOT_PLAYER, new SbrSprite( PLSHOT_TEX, 24, 24 ), BLASTER_DMG, BLASTER_SPEED, this );
	_pMainWpn[2] = new Shot( SHOT_PLAYER, new SbrSprite( PLSHOT_TEX, 24, 24 ), BLASTER_DMG, BLASTER_SPEED, this );

	Speed = 250;
	SetMoveType( new MV_TRJ_STRAIGHT( &_ptPos, Speed ) );
}

//-------------------------------------------------------------

PlayerShip::~PlayerShip()
{
	delete _pMainWpn[0];
	delete _pMainWpn[1];
	delete _pMainWpn[2];
	delete _pRockets;
}

//-------------------------------------------------------------

void PlayerShip::Process( float fTimeDelta )
{
	MovingObject::Process( fTimeDelta );

	if ( _bMainWpnReload )
	{
		if ( _fMainRelTime <= 0 ) 
			_bMainWpnReload = false;
		else
			_fMainRelTime -= fTimeDelta;
	}
	if ( _bRocketsReload )
	{
		if ( _fRocketsRelTime <= 0 )
			_bRocketsReload = false;
		else
			_fRocketsRelTime -= fTimeDelta;
	}
}

//-------------------------------------------------------------

void PlayerShip::OnCollide( Object *obj )
{
	if ( ( obj->GetID() == SHOT_ENEMY || obj->GetID() == SHOT_ENEMY_FST ) && IsVisible() )
	{
		Hit( ((Shot*)obj)->GetDamage() );
	}
	if ( obj->GetID() == ENEMY01 && IsVisible() )
	{
		Hit( 500 );
	}
}

//-------------------------------------------------------------

int PlayerShip::GetCost()
{
	int i = _nCost;
	_nCost = 0;
	return i;
}

//-------------------------------------------------------------

void PlayerShip::StrafeLeft()
{
	if ( !_nCurArmor )
		return;

	MoveTo( 25, GetPosY() );
}

//-------------------------------------------------------------

void PlayerShip::StrafeRight()
{
	if ( !_nCurArmor )
		return;

	MoveTo( 775, GetPosY() );
}

//-------------------------------------------------------------

void PlayerShip::Shoot()
{
	if ( _bMainWpnReload ) 
		return;

	_pMainWpn[_nCurMainShot % 3]->Shoot( _ptPos.x, _ptPos.y, _ptPos.x, -50 );
	_nCurMainShot++;
	_bMainWpnReload = true;
	_fMainRelTime = BLASTER_RELOAD_TIME;

	AzuCreate( pAzu );
	pAzu->Play( PLSHOT_MX );
	pAzu->Release();
}

//-------------------------------------------------------------

void PlayerShip::ShootRocket()
{
	if ( _bRocketsReload || !_nRocketsCount )
		return;

	_pRockets->Shoot( _ptPos.x, _ptPos.y, _ptPos.x, -50 );
	_bRocketsReload = true;
	_nRocketsCount--;
	_fRocketsRelTime = ROCKETS_RELOAD_TIME;

	AzuCreate( pAzu );
	pAzu->Play( ROCKET_MX );
	pAzu->Release();
}

//-------------------------------------------------------------