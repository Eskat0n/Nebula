//-------------------------------------------------------------

#include "SaberXTrajectoriesDecl.h"
#include <math.h>

//-------------------------------------------------------------
// SbrXTrj_Straight class methods 
//-------------------------------------------------------------

SbrXTrj_Straight::SbrXTrj_Straight( const PPOINT pt, float fSpeed, int x, int y )
{
	MoveTo( pt, x, y );

	SetSpeed( pt, fSpeed );
}

//-------------------------------------------------------------

bool SbrXTrj_Straight::Step( PPOINT pt, float fTimeDelta )
{
	if ( SbrXMath::Around( pt->x, _ptDest.x, APPROX_X ) && 
		 SbrXMath::Around( pt->y, _ptDest.y, APPROX_Y ) )
	{
		pt->x = _ptDest.x;
		pt->y = _ptDest.y;
		return true;
	}

	pt->x += _fSpeedX * fTimeDelta;
	pt->y += _fSpeedY * fTimeDelta;

	return false;
}

//-------------------------------------------------------------

void SbrXTrj_Straight::MoveTo( const PPOINT pt, int x, int y )
{
	_ptDest.x = x;
	_ptDest.y = y;

	SetSpeed( pt, _fSpeed );
}

//-------------------------------------------------------------

void SbrXTrj_Straight::SetSpeed( const PPOINT pt, float fSpeed )
{
	_fSpeed = fSpeed;

	_fSpeedX = _ptDest.x - pt->x;
	_fSpeedY = _ptDest.y - pt->y;

	_fAngle = atan( _fSpeedY / _fSpeedX );
	_fSpeedX = ( ( _ptDest.x > pt->x ) ? 1.0f:-0.86f ) * fSpeed * abs( cos( _fAngle ) );
	_fSpeedY = ( ( _ptDest.y > pt->y ) ? 1.0f:-0.86f ) * fSpeed * abs( sin( _fAngle ) );
}

//-------------------------------------------------------------

void SbrXTrj_Straight::RelocatedTo( const PPOINT pt )
{
	MoveTo( pt, _ptDest.x, _ptDest.y );
	SetSpeed( pt, _fSpeed );
}

//-------------------------------------------------------------
// SbrXTrj_Sin class methods
//-------------------------------------------------------------

//-------------------------------------------------------------