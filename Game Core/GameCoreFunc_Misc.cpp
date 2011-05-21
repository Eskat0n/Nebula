//-------------------------------------------------------------

#include "../Azumaria Sound Engine/AzuEngineClass.h"

#include "GameCoreClass_Misc.h"
#include "GameDeclarations.h"

//-------------------------------------------------------------

Combo::Combo( int No )
{
	_pMessage = new SbrHUDMessage();

	_pMessage->SetFont( L"Arial", 28 );
	_pMessage->SetFontColor( SBR_RED );
	_pMessage->Hide();

	_nCurLevel = -1;
	_fTimeElapsed = 0;

	if ( No == 2 )
		_pMessage->SetPos( 500, 50 );
	else
		_pMessage->SetPos( 200, 50 );
}

//-------------------------------------------------------------

Combo::~Combo()
{
	delete _pMessage;
}

//-------------------------------------------------------------

void Combo::Process( float fTimeDelta )
{
	if ( _fTimeElapsed <= 0 )	
	{ 
		_nCurLevel = -1;
		_pMessage->Hide();
		return;
	}
	_fTimeElapsed -= fTimeDelta;
}

//-------------------------------------------------------------

void Combo::Increase()
{
	if ( _fTimeElapsed <= 0 && _nCurLevel != -1 )
		return;

	switch ( _nCurLevel )
	{
		case -1:
			_fTimeElapsed = 2.5f;
			_nCurLevel++;
			break;
		case 0:
			_fTimeElapsed = 2.4f;
			_nCurLevel++;
			_pMessage->SetText( COMBOLVL01 );
			_pMessage->Show();
			break;
		case 1:
			_fTimeElapsed = 2.3f;
			_pMessage->SetText( COMBOLVL02 );
			_nCurLevel++;
			break;
		case 2:
			_fTimeElapsed = 2.2f;
			_pMessage->SetText( COMBOLVL03 );
			_nCurLevel++;
			break;
		case 3:
			_fTimeElapsed = 2.1f;
			_pMessage->SetText( COMBOLVL04 );
			_nCurLevel++;
			break;
		case 4:
			_fTimeElapsed = 1.5f;
			_pMessage->SetText( COMBOLVL05 );
			_nCurLevel++;
			break;
		case 5:
			_fTimeElapsed = 1.5f;
	}
}

//-------------------------------------------------------------

void Combo::Render()
{
	_pMessage->Draw();
}

//-------------------------------------------------------------

float Combo::GetRatio() const
{
	switch ( _nCurLevel )
	{
		case 1: return 1.5f;
		case 2: return 2.0f;
		case 3: return 3.0f;
		case 4: return 3.5f;
		case 5: return 4.0f;

		default:
			return 1.0f;
	}
}

//-------------------------------------------------------------

BossShip::BossShip()
		: EnemyShip( ENEMY_BOSS, new SbrSprite( BOSS_TEX, 256, 256 ), BOSS_ARMOR, 0, 0 )
{
	SetPos( 400, 150 );
	SetHotSpot( 128, 128 );
	SetHitbox( 28, 28, 200, 180 );

	SbrSprite* spr = new SbrSprite( BOSS_SHOT_TEX, 48, 48 );
	_pWpn = new Shot( SHOT_ENEMY, spr, 1, BOSS_SHOT_SPEED, this );
	_pWpn->SetHotSpot( 24, 24 );
	_pWpn->SetHitbox( 8, 8, 32, 32 );
	_pAddWpns[0] = new Shot( SHOT_ENEMY, spr, 1, BOSS_SHOT_SPEED, this );
	_pAddWpns[0]->SetHotSpot( 24, 24 );
	_pAddWpns[0]->SetHitbox( 8, 8, 32, 32 );
	_pAddWpns[1] = new Shot( SHOT_ENEMY, spr, 1, BOSS_SHOT_SPEED, this );
	_pAddWpns[1]->SetHotSpot( 24, 24 );
	_pAddWpns[1]->SetHitbox( 8, 8, 32, 32 );
	_pAddWpns[2] = new Shot( SHOT_ENEMY, spr, 1, BOSS_SHOT_SPEED, this );
	_pAddWpns[2]->SetHotSpot( 24, 24 );
	_pAddWpns[2]->SetHitbox( 8, 8, 32, 32 );

	_pMegaBlaster = new MegaBlaster();

	_fShootChance = BOSS_SHOOT_CHANCE;
	_fAbilityChance = BOSS_ABL_CHANCE;
	_nCost = BOSS_COST;

	_fMBCooldown = BOSS_ACOOLDOWN;
}

//-------------------------------------------------------------

BossShip::~BossShip()
{
	delete _pMegaBlaster;
}

//-------------------------------------------------------------

void BossShip::Process( float fTimeDelta )
{
	MovingObject::Process( fTimeDelta );

	if ( _bReload )
		if ( _fWpnReloadTime <= 0 )
			_bReload = false;
		else 
			_fWpnReloadTime -= fTimeDelta;

	if ( _bAbilityReload )
	{
		if ( _fAbReloadTime <= 0 )
		{
			_bAbilityReload = false;
			_fMBCooldown = BOSS_ACOOLDOWN;
			_pMegaBlaster->Disappear();
		}
		else
			_fAbReloadTime -= fTimeDelta;
	}
	else
	{
		if ( _fMBCooldown <= 0 )
			_fMBCooldown = -2;
		else
			_fMBCooldown -= fTimeDelta;
	}

	_pMegaBlaster->Update( _ptPos.x + 4, _ptPos.y + 63 );
}

//-------------------------------------------------------------

void BossShip::Shoot( bool forSure )
{
	if ( forSure || ( ( _fShootChance >= SbrXRandom::RandomChance() ) && !_bReload ) )
	{
		_pWpn->Shoot( _ptPos.x - 64, _ptPos.y + 100, _ptPos.x - 64, 630 );
		_pAddWpns[0]->Shoot( _ptPos.x - 90, _ptPos.y + 100, _ptPos.x - 90, 630 );
		_pAddWpns[1]->Shoot( _ptPos.x + 80, _ptPos.y + 100, _ptPos.x + 80, 630 );
		_pAddWpns[2]->Shoot( _ptPos.x + 106, _ptPos.y + 100, _ptPos.x + 106, 630 );

		_bReload = true;
		_fWpnReloadTime = BOSS_RELOAD_TIME;

		AzuCreate( pAzu );
		pAzu->Play( BOSS_SHOT_MX );
		pAzu->Release();
	}
}

//-------------------------------------------------------------

void BossShip::UseAbility( bool forSure )
{
	if ( forSure || ( ( _fAbilityChance >= SbrXRandom::RandomChance() ) && !_bAbilityReload && _fMBCooldown == -2 ) )
	{
		_pMegaBlaster->Blast( _ptPos.x + 4, _ptPos.y + 63 );

		_bAbilityReload = true;
		_fAbReloadTime = BOSS_ARELOAD_TIME;
	}
}

//-------------------------------------------------------------

void BossShip::OnCollide( Object* obj )
{
	if ( ( obj->GetID() == SHOT_PLAYER || obj->GetID() == SHOT_PLAYER_ROCKET ) && IsVisible() )
	{
		if ( !Hit( ((Shot*)obj)->GetDamage() ) )
			((Shot*)obj)->GetOwner()->AddCost( GetCost() );
	}
}

//-------------------------------------------------------------

void BossShip::StrafeLeft()
{
	if ( _bIndependent )
		return;

	MoveTo( 250, _ptPos.y );
}

//-------------------------------------------------------------

void BossShip::StrafeRight()
{
	if ( _bIndependent )
		return;

	MoveTo( 550, _ptPos.y );
}

//-------------------------------------------------------------

MegaBlaster::MegaBlaster()
		: StaticObject( DUMMY )
{
	_pSpr = new SbrSprite( MEGABLAST_TEX, 256, 512 );

	SetHotSpot( 128, 0 );
	SetHitbox( 32, 0, 188, 512 );

	Hide();

	_fFadeTime = -2;
}

//-------------------------------------------------------------

MegaBlaster::~MegaBlaster()
{ }

//-------------------------------------------------------------

void MegaBlaster::Process( float fTimeDelta )
{
	if ( _fFadeTime == -2 )
		return;

	if ( _fFadeTime <= 0 )
	{
		if ( !_bFadeIn )
		{
			Hide();
			return;
		}

		_fFadeTime = -2;
		_pSpr->SetColor( SBR_WHITE );
		_nID = SHOT_ENEMY;
		return;
	}

	// Proceed fading effect
	// Slowly, slowly we are increasing/decreasing value of alpha-channel to create nicy looking beam
	_pSpr->SetColor( 0x00ffffff + ( 0xff000000 & 
		(DWORD)( ( ( _bFadeIn ) ? ( 1 - _fFadeTime / MB_FULL_FADE ):( _fFadeTime / MB_FULL_FADE ) ) 
		* 0xff000000 ) ) );
	_fFadeTime -= fTimeDelta;
}

//-------------------------------------------------------------

void MegaBlaster::OnCollide( Object* obj )
{
}

//-------------------------------------------------------------

void MegaBlaster::Blast( int x, int y )
{
	SetPos( x, y );
	Show();
	_pSpr->SetColor( 0x00ffffff );

	_bFadeIn = true;
	_fFadeTime = MB_FULL_FADE;

	AzuCreate( pAzu );
	pAzu->Play( MEGABLAST_MX );
	pAzu->Release();
}

//-------------------------------------------------------------

void MegaBlaster::Update( int x, int y )
{
	SetPos( x, y );
}

//-------------------------------------------------------------

void MegaBlaster::Disappear()
{
	_nID = DUMMY;

	_bFadeIn = false;
	_fFadeTime = MB_FULL_FADE;
}

//-------------------------------------------------------------