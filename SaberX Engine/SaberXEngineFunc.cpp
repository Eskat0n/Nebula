//-------------------------------------------------------------

#include "SaberXEngineClass.h"

#include <boost/foreach.hpp>

//-------------------------------------------------------------

World*	World::_self = 0;
int		World::_nRefCount = 0;

//-------------------------------------------------------------
// World methods
//-------------------------------------------------------------

World* World::Instance( int w, int h, int x, int y )
{
	if ( !_self )
		_self = new World( w, h, x, y );
	_nRefCount++;
	return _self;
}

//-------------------------------------------------------------

void World::Release()
{
	_nRefCount--;
	if ( !_nRefCount )
	{
		delete _self;
		_self = 0;
	}
}

//-------------------------------------------------------------

World::World( int w, int h, int x, int y )
{
	_rcBorders.left = x;
	_rcBorders.top = y;
	_rcBorders.right = x + w;
	_rcBorders.bottom = y + h;
}

//-------------------------------------------------------------

World::~World()
{
	BOOST_FOREACH( Object* pObj, _vObjects )
	{
		delete pObj;
	}
	_vObjects.clear();
}

//-------------------------------------------------------------

void World::AddObj( Object* obj )
{
	_vObjects.push_back( obj );
}

//-------------------------------------------------------------

void World::DeleteObj( Object* obj )
{
	std::vector<Object*>::iterator iTemp;

	for ( iTemp = _vObjects.begin(); iTemp != _vObjects.end(); iTemp++ )
	{
		if ( *iTemp == obj )
		{
			_vObjects.erase( iTemp );
			break;
		}
	}
}

//-------------------------------------------------------------

void World::ProcessAll( float fTimeDelta )
{
	BOOST_FOREACH( Object* pObj, _vObjects )
	{
		pObj->Process( fTimeDelta );

		BOOST_FOREACH( Object* pSubj, _vObjects )
		{
			if ( pObj->Collide( pSubj ) && ( pObj != pSubj ) )
			{
				pObj->OnCollide( pSubj );
				pSubj->OnCollide( pObj );
			}
		}
	}
}

//-------------------------------------------------------------

void World::RenderAll()
{
	BOOST_FOREACH( Object* pObj, _vObjects )
	{
		pObj->Render();
	}
}

//-------------------------------------------------------------

void World::GetBorders( RECT* rect ) const
{
	memcpy( rect, &_rcBorders, sizeof(_rcBorders) );
}

//-------------------------------------------------------------
// Object methods
//-------------------------------------------------------------

Object::Object( int id, int x, int y )
		: _nID( id ), 
		  _pWorld( World::Instance() ),
		  _pSpr( 0 ),
		  _bVisible( true )
{
	_pWorld = World::Instance();

	_pWorld->AddObj( this );

	_ptPos.x = x;
	_ptPos.y = y;
	_ptHotSpot.x =
	_ptHotSpot.y = 0;

	_rcHitbox.left = 0;
	_rcHitbox.top = 0;
	_rcHitbox.right = 1;
	_rcHitbox.bottom = 1;
}

//-------------------------------------------------------------

Object::~Object()
{
	if ( _pSpr )
		delete _pSpr;

	_pWorld->DeleteObj( this );
	_pWorld->Release();
}

//-------------------------------------------------------------

bool Object::Collide( Object* obj )
{
	RECT dummy, rcThisHB, rcTargetHB;

	// HORRIBLE!
	CopyRect( &rcThisHB, &_rcHitbox );
	OffsetRect( &rcThisHB, _ptPos.x, _ptPos.y );
	CopyRect( &rcTargetHB, obj->GetHitbox() );
	OffsetRect( &rcTargetHB, obj->GetPosX(), obj->GetPosY() );

	return IntersectRect( &dummy, &rcThisHB, &rcTargetHB );
}

//-------------------------------------------------------------

void Object::Render()
{
	if ( _bVisible && _pSpr )
		_pSpr->Draw( _ptPos.x, _ptPos.y );
}

//-------------------------------------------------------------

void Object::SetSprite( SbrSprite* spr )
{
	if ( _pSpr && _pSpr != spr )
		delete _pSpr;

	_pSpr = spr;
	_pSpr->SetHotSpot( _ptHotSpot.x, _ptHotSpot.y );
}

//-------------------------------------------------------------

void Object::SetHitbox( RECT *rect )
{
	_rcHitbox.left		= rect->left;
	_rcHitbox.top		= rect->top;
	_rcHitbox.right		= rect->right;
	_rcHitbox.bottom	= rect->bottom;
}

//-------------------------------------------------------------

void Object::SetHitbox( int x, int y, int w, int h )
{
	_rcHitbox.left		= x - _ptHotSpot.x;
	_rcHitbox.top		= y - _ptHotSpot.y;
	_rcHitbox.right		= x + w - _ptHotSpot.x;
	_rcHitbox.bottom	= y + h - _ptHotSpot.y;
}

//-------------------------------------------------------------

void Object::SetHotSpot( int x, int y )
{
	if ( _pSpr ) _pSpr->SetHotSpot( x, y );

	_rcHitbox.left		-= x - _ptHotSpot.x;
	_rcHitbox.top		-= y - _ptHotSpot.y;
	_rcHitbox.right		-= x - _ptHotSpot.x;
	_rcHitbox.bottom	-= y - _ptHotSpot.y;

	_ptHotSpot.x = x;
	_ptHotSpot.y = y;
}

//-------------------------------------------------------------
// Moving Object methods
//-------------------------------------------------------------

MovingObject::MovingObject( int id, int x, int y, PMV_TRJ mvType, float spd, BND_REL bnd )
		: Object( id, x, y ),
		  _pMovTrj( mvType ),
		  _fSpeed( spd ),
		  _boundary( bnd ),
		  _fFriction( 0 ),
		  _bMoving( mvType != 0 )
{
	if ( _pMovTrj )
		_pMovTrj->SetSpeed( &_ptPos, spd );
}

//-------------------------------------------------------------

MovingObject::~MovingObject()
{
	if ( _pMovTrj )
		delete _pMovTrj;
}

//-------------------------------------------------------------

void MovingObject::SetMoveType( PMV_TRJ mvType )
{
	if ( _pMovTrj )
		delete _pMovTrj;

	_pMovTrj = mvType;

	if ( !mvType )
		Stop();
}

//-------------------------------------------------------------

void MovingObject::SetSpeed( FLOAT fSpeed )
{
	_fSpeed = fSpeed;

	if ( fSpeed == 0 )
		Stop();

	if ( _pMovTrj )
		_pMovTrj->SetSpeed( &_ptPos, fSpeed );
}

//-------------------------------------------------------------

void MovingObject::MoveTo( int mvx, int mvy )
{
	if ( _pMovTrj )
	{
		_bMoving = true;
		_pMovTrj->MoveTo( &_ptPos, mvx, mvy );
	}
}

//-------------------------------------------------------------

void MovingObject::Continue()
{
	if ( _pMovTrj )
		_pMovTrj->RelocatedTo( &_ptPos );
}

//-------------------------------------------------------------

void MovingObject::Process( FLOAT fTimeDelta )
{
	if ( !_pMovTrj )
		return;

	if ( _bMoving )
		if ( _pMovTrj->Step( &_ptPos, fTimeDelta ) )
			Stop();

	// TODO: put some code here to check exceeding the boundaries

	// reserved
	switch ( _boundary )
	{
		case SBRX_BND_IGNORE: break;
		case SBRX_BND_MIRROR: break;	
		case SBRX_BND_BOUNCE: break;
	}
}

//-------------------------------------------------------------
// Static Object methods
//-------------------------------------------------------------

StaticObject::StaticObject( int id, int x, int y )
		: Object ( id, x, y )
{ }

//-------------------------------------------------------------
// Effect methods
//-------------------------------------------------------------

Effect::Effect( int id, SbrAnim* anim )
		: Object( id ),
		  _pAnim( anim )
{
	_pSpr = 0;

	Hide();
	if ( _pAnim )
		_pAnim->SetMode( SBRAP_NOLOOP );
}

//-------------------------------------------------------------

Effect::~Effect()
{
	if ( _pAnim )
		delete _pAnim;
}

//-------------------------------------------------------------

void Effect::Perform( int x, int y )
{
	if ( !_pAnim )
		return;

	if ( !_bVisible )
	{
		SetPos( x, y );
		_pAnim->Play();
		_bVisible = true;
	}
}

//-------------------------------------------------------------

void Effect::Stop()
{
	if ( !_pAnim )
		return;

	if ( _bVisible )
	{
		_pAnim->Stop();
		_pAnim->SetFrame(0);
		_bVisible = false;
	}
}

//-------------------------------------------------------------

void Effect::OnCollide( Object* obj )
{ }

//-------------------------------------------------------------

void Effect::Process( FLOAT fTimeDelta )
{
	if ( !_pAnim )
		return;

	if ( _bVisible )
	{
		if ( !_pAnim->IsPlaying() )
			_bVisible = false;
		_pAnim->Update( fTimeDelta );
	}
}

//-------------------------------------------------------------

void Effect::Render()
{
	if ( !_pAnim )
		return;

	if ( _bVisible )
		_pAnim->Draw( _ptPos.x, _ptPos.y );
}

//-------------------------------------------------------------

void Effect::SetAnim( SbrAnim *anim )
{ 
	if ( anim && anim != _pAnim ) delete _pAnim; _pAnim = anim; 
}

//-------------------------------------------------------------