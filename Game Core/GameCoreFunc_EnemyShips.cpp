//-------------------------------------------------------------
// Nebula Game Core enemy ships classes methods implementation file
//-------------------------------------------------------------

#include "GameCoreClass_Ships.h"

//-------------------------------------------------------------
// Enemy Ship methods
//-------------------------------------------------------------

EnemyShip::EnemyShip( int id, SbrSprite* spr, int maxarm, int arX, int arY )
		: Ship( maxarm )
{ 
	_bReload		=
	_bAbilityReload = false;
	_bIndependent	= false;

	_nID = id;
	SetSprite( spr );
	SetHotSpot( 24, 24 );

	Speed = 150;
	_ptArmadePos.x = arX;
	_ptArmadePos.y = arY;

	SetPos( 175 + 50 * arX, 125 + 50 * arY );

	SetMoveType( new MV_TRJ_STRAIGHT( &_ptPos, Speed ) );
}

//-------------------------------------------------------------

EnemyShip::~EnemyShip()
{
	if ( _pWpn )	delete _pWpn;
}

//-------------------------------------------------------------

void EnemyShip::StrafeLeft()
{
	if ( _bIndependent )
		return;

	MoveTo( 25 + 50 * _ptArmadePos.x, _ptPos.y );
}

//-------------------------------------------------------------

void EnemyShip::StrafeRight()
{
	if ( _bIndependent )
		return;

	MoveTo( 325 + 50 * _ptArmadePos.x, _ptPos.y );
}

//-------------------------------------------------------------

void EnemyShip::Destroy()
{
	Ship::Destroy();
	_bIndependent = true;
}

//-------------------------------------------------------------
// Pod methods
//-------------------------------------------------------------

Pod::Pod( SbrSprite* spr, int arX, int arY )
		: EnemyShip( ENEMY01, spr, POD_ARMOR, arX, arY )
{	
	_pWpn = new Shot( SHOT_ENEMY, new SbrSprite( ENEMYSHOT_TEX, 16, 16 ), 1, POD_SHOT_SPEED, this );

	_fShootChance = POD_SHOOT_CHANCE;
	_fAbilityChance = POD_ABL_CHANCE;
	_nCost = POD_COST;
}

//-------------------------------------------------------------

void Pod::Process( float fTimeDelta )
{
	MovingObject::Process( fTimeDelta );
	if ( _bReload )
		if ( _fWpnReloadTime <= 0 )
			_bReload = false;
		else 
			_fWpnReloadTime -= fTimeDelta;

	if ( _bAbilityReload )
		if ( _fAbReloadTime <= 0 )
			_bAbilityReload = false;
		else 
			_fAbReloadTime -= fTimeDelta;

	if ( _ptPos.y <= 610 && _ptPos.y >= 550 )
		Destroy();
}

//-------------------------------------------------------------

void Pod::Shoot( bool forSure )
{
	if ( forSure || ( ( _fShootChance >= SbrXRandom::RandomChance() ) && !_bReload ) )
	{
		_pWpn->Shoot( _ptPos.x, _ptPos.y, _ptPos.x, 610 );
		_bReload = true;
		_fWpnReloadTime = POD_RELOAD_TIME;
	}
}

//-------------------------------------------------------------

void Pod::UseAbility( bool forSure )
{
	if ( !_bVisible )
		return;

	if ( forSure || ( ( _fAbilityChance >= SbrXRandom::RandomChance() ) && !_bAbilityReload ) )
	{
		_bIndependent = true;

		_nCost = 1000;

		Stop();
		Speed = 600.0f;
		MoveTo( _ptPos.x, 610 );

		_bAbilityReload = true;
		_fAbReloadTime = POD_ARELOAD_TIME;
	}
}

//-------------------------------------------------------------

void Pod::OnCollide( Object* obj )
{
	if ( ( obj->GetID() == SHOT_PLAYER || obj->GetID() == SHOT_PLAYER_ROCKET ) && IsVisible() )
	{ 
		if ( !Hit( ((Shot*)obj)->GetDamage() ) )
			((Shot*)obj)->GetOwner()->AddCost( GetCost() );
	}

	if ( obj->GetID() == PLSHIP01 && IsVisible() )
		Destroy();
}

//-------------------------------------------------------------

//void Pod::Destroy()
//{
//}

//-------------------------------------------------------------
// Borg methods
//-------------------------------------------------------------

Borg::Borg( SbrSprite* spr, int arX, int arY )
		: EnemyShip( ENEMY02, spr, BORG_ARMOR, arX, arY )
{
	_pWpn = new Shot( SHOT_ENEMY, new SbrSprite( ENEMYSHOT_TEX, 16, 16 ), 1, BORG_SHOT_SPEED, this );

	_fShootChance = BORG_SHOOT_CHANCE;
	_fAbilityChance = BORG_ABL_CHANCE;
	_nCost = BORG_COST;
}

//-------------------------------------------------------------

void Borg::Process( float fTimeDelta )
{
	MovingObject::Process( fTimeDelta );

	if ( _bReload )
		if ( _fWpnReloadTime <= 0 )
			_bReload = false;
		else 
			_fWpnReloadTime -= fTimeDelta;

	if ( _bAbilityReload )
		if ( _fAbReloadTime <= 0 )
		{
			_bAbilityReload = false;
			ChangeColor( SBR_WHITE );
		}
		else 
			_fAbReloadTime -= fTimeDelta;
}

//-------------------------------------------------------------

void Borg::Shoot( bool forSure )
{
	if ( forSure || ( ( _fShootChance >= SbrXRandom::RandomChance() ) && !_bReload ) )
	{
		_pWpn->Shoot( _ptPos.x, _ptPos.y, _ptPos.x, 610 );
		_bReload = true;
		_fWpnReloadTime = BORG_RELOAD_TIME;

		SbrXRandom::Seed();
	}
}

//-------------------------------------------------------------

void Borg::UseAbility( bool forSure )
{
	if ( forSure || ( ( _fAbilityChance >= SbrXRandom::RandomChance() ) && !_bAbilityReload ) )
	{
		// Set transparency
		ChangeColor( 0x44ffffff );
		SbrXRandom::Seed();

		_bAbilityReload = true;
		_fAbReloadTime = BORG_ARELOAD_TIME;
	}
}

//-------------------------------------------------------------

void Borg::OnCollide( Object* obj )
{
	if ( ( obj->GetID() == SHOT_PLAYER || obj->GetID() == SHOT_PLAYER_ROCKET ) && IsVisible() )
	{ 
		if ( !Hit( ((Shot*)obj)->GetDamage() ) )
			((Shot*)obj)->GetOwner()->AddCost( GetCost() );
	}
}

//-------------------------------------------------------------

//void Borg::Destroy()
//{
//}

//-------------------------------------------------------------
// Tres methods
//-------------------------------------------------------------

Tres::Tres( SbrSprite* spr, int arX, int arY )
		: EnemyShip( ENEMY03, spr, TRES_ARMOR, arX, arY )
{
	_pWpn = new Shot( SHOT_ENEMY, new SbrSprite( ENEMYSHOT_TEX, 16, 16 ), 1, TRES_SHOT_SPEED, this );

	_fShootChance = TRES_SHOOT_CHANCE;
	_fAbilityChance = TRES_ABL_CHANCE;
	_nCost = TRES_COST;

	_pAbWpn[0] = new Shot( SHOT_ENEMY_FST, new SbrSprite( ENEMYSHOT_TEX, 16, 16 ), 1, TRES_SFST_SPEED, this );
	_pAbWpn[1] = new Shot( SHOT_ENEMY_FST, new SbrSprite( ENEMYSHOT_TEX, 16, 16 ), 1, TRES_SFST_SPEED, this );
	_pAbWpn[2] = new Shot( SHOT_ENEMY_FST, new SbrSprite( ENEMYSHOT_TEX, 16, 16 ), 1, TRES_SFST_SPEED, this );
}

//-------------------------------------------------------------

Tres::~Tres()
{
	delete _pAbWpn[0];
	delete _pAbWpn[1];
	delete _pAbWpn[2];
}

//-------------------------------------------------------------

void Tres::Process( float fTimeDelta )
{
	MovingObject::Process( fTimeDelta );
	if ( _bReload )
		if ( _fWpnReloadTime <= 0 )
			_bReload = false;
		else 
			_fWpnReloadTime -= fTimeDelta;

	if ( _bAbilityReload )
		if ( _fAbReloadTime <= 0 )
			_bAbilityReload = false;
		else 
			_fAbReloadTime -= fTimeDelta;
}

//-------------------------------------------------------------

void Tres::Shoot( bool forSure )
{
	if ( forSure || ( ( _fShootChance >= SbrXRandom::RandomChance() ) && !_bReload ) )
	{
		_pWpn->Shoot( _ptPos.x, _ptPos.y, _ptPos.x, 610 );
		_bReload = true;
		_fWpnReloadTime = TRES_RELOAD_TIME;
	}
}

//-------------------------------------------------------------

void Tres::UseAbility( bool forSure )
{
	if ( forSure || ( ( _fAbilityChance >= SbrXRandom::RandomChance() ) && !_bAbilityReload ) )
	{
		_pAbWpn[0]->Shoot( _ptPos.x, _ptPos.y, _ptPos.x, 610 );
		_pAbWpn[1]->Shoot( _ptPos.x + 9, _ptPos.y, _ptPos.x + 9, 610 );
		_pAbWpn[2]->Shoot( _ptPos.x - 9, _ptPos.y, _ptPos.x - 9, 610 );

		_bAbilityReload = true;
		_fAbReloadTime = TRES_ARELOAD_TIME;
	}
}

//-------------------------------------------------------------

void Tres::OnCollide( Object* obj )
{
	if ( ( obj->GetID() == SHOT_PLAYER || obj->GetID() == SHOT_PLAYER_ROCKET ) && IsVisible() )
	{ 
		if ( !Hit( ((Shot*)obj)->GetDamage() ) )
			((Shot*)obj)->GetOwner()->AddCost( GetCost() );
	}
}

//-------------------------------------------------------------

//void Tres::Destroy()
//{
//}

//-------------------------------------------------------------
// Gungir methods
//-------------------------------------------------------------

Gungir::Gungir( SbrSprite* spr, int arX, int arY )
		: EnemyShip( ENEMY04, spr, GUNGIR_ARMOR, arX, arY )
{
	_pWpn = new Shot( SHOT_ENEMY, new SbrSprite( ENEMYSHOT_TEX, 16, 16 ), 1, GUNGIR_SHOT_SPEED, this );

	_fShootChance = GUNGIR_SHOOT_CHANCE;
	_fAbilityChance = GUNGIR_ABL_CHANCE;
	_nCost = GUNGIR_COST;
}

//-------------------------------------------------------------

void Gungir::Process( float fTimeDelta )
{
	MovingObject::Process( fTimeDelta );

	if ( _bReload )
		if ( _fWpnReloadTime <= 0 )
			_bReload = false;
		else 
			_fWpnReloadTime -= fTimeDelta;

	if ( _bAbilityReload )
		if ( _fAbReloadTime <= 0 )
			_bAbilityReload = false;
		else 
			_fAbReloadTime -= fTimeDelta;
}

//-------------------------------------------------------------

void Gungir::Shoot( bool forSure )
{
	if ( forSure || ( ( _fShootChance >= SbrXRandom::RandomChance() ) && !_bReload ) )
	{
		_pWpn->Shoot( _ptPos.x, _ptPos.y, _ptPos.x, 610 );
		_bReload = true;
		_fWpnReloadTime = GUNGIR_RELOAD_TIME;
	}
}

//-------------------------------------------------------------

void Gungir::UseAbility( bool forSure )
{
	if ( forSure || ( ( _fAbilityChance >= SbrXRandom::RandomChance() ) && !_bAbilityReload ) )
	{
		_bAbilityReload = true;
		_fAbReloadTime = GUNGIR_ARELOAD_TIME;
	}
}

//-------------------------------------------------------------

void Gungir::OnCollide( Object* obj )
{
	if ( ( obj->GetID() == SHOT_PLAYER || obj->GetID() == SHOT_PLAYER_ROCKET ) && IsVisible() )
	{ 
		if ( !Hit( ((Shot*)obj)->GetDamage() ) )
			((Shot*)obj)->GetOwner()->AddCost( GetCost() );
	}
}

//-------------------------------------------------------------

//void Gungir::Destroy()
//{
//}

//-------------------------------------------------------------