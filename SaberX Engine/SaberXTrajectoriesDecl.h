//-------------------------------------------------------------

#include "SaberXMath.h"

#ifndef _SBRX_TRAJECTORIES_
#define _SBRX_TRAJECTORIES_

//-------------------------------------------------------------

#define APPROX_X 3
#define APPROX_Y 3

//-------------------------------------------------------------

// Abstract class which handles 
class SbrXTrajectory
{
public:
	/**
	 * Performs next step in motion key-position calculation.
	 * @param pt pointer to POINT struct which contains current coordinates of moving object and will be filled with new coordinate values when step calculations are finished.
	 * @param fTimeDelta time elapsed since last step (in msc's).
	 * @return true if moving are finished, otherwise false.
	 */
	virtual	bool	Step( PPOINT pt, float fTimeDelta ) = 0;
	/**
	 * Changes destination point to specified one.
	 * @param pt pointer to POINT struct which contains current coordinates of moving object.
	 * @param x coordinate of destination point along x axis.
	 * @param y coordinate of destination point along y axis.
	 */
	virtual void	MoveTo( const PPOINT pt, int x, int y ) = 0;
	/**
	 * Sets linear or arc speed (current trajectory dependent).
	 * @param pt pointer to POINT struct which contains current coordinates of moving object.
	 * @param fSpeed value of speed.
	 */
	virtual void	SetSpeed( const PPOINT pt, float fSpeed ) = 0;
	/**
	 * Notifies trajectory calculator that object somehow relocated to specified position.
	 * @param pt pointer to POINT struct which contains current coordinates of moving object.
	 */
	virtual	void	RelocatedTo( const PPOINT pt ) = 0;
};

typedef SbrXTrajectory	MV_TRJ;
typedef SbrXTrajectory* PMV_TRJ;

//-------------------------------------------------------------
// Straight move trajectory (point-to-point)
//-------------------------------------------------------------

/**
 * Concrete implementation of SbrXTrajectory to perform rectilinear motion from point to point with no acceleration and friction.
 * NOTE: constant approximation used in this class is really horrible and should be replaced by dynamical approximation.
 */
class SbrXTrj_Straight: public SbrXTrajectory
{
private:
	FLOAT	_fSpeed;					/**< Linear speed itself. */
	FLOAT	_fSpeedX;					/**< Speed projected at x axis. */
	FLOAT	_fSpeedY;					/**< Speed projected at y axis. */
	FLOAT	_fAngle;					/**< Angle between motion line and x axis. */
	POINT	_ptDest;					/**< Coordinates of motion destination point. */

public:
	/**
	 * Trajectory constructor.
	 * @param pt pointer to POINT struct which contains current coordinates of moving object.
	 * @param fSpeed value of linear speed.
 	 * @param to_x coordinate of destination point along x axis.
	 * @param to_y coordinate of destination point along y axis.
	 */
	SbrXTrj_Straight( const PPOINT pt, float fSpeed = 0, int to_x = 0, int to_y = 0 );

	bool	Step( PPOINT pt, float fTimeDelta );
	void	MoveTo( const PPOINT pt, int x, int y );
	void	SetSpeed( const PPOINT pt, float fSpeed );
	void	RelocatedTo( const PPOINT pt );
};

typedef SbrXTrj_Straight	MV_TRJ_STRAIGHT;
typedef SbrXTrj_Straight*	PMV_TRJ_STRAIGHT;

//-------------------------------------------------------------
// Sinusoidal move trajectory (point-to-point)
//-------------------------------------------------------------

//class SbrXTrj_Sin: public SbrXTrajectory
//{
//private:
//	FLOAT	_fFreq, _fAmp, _fPhase;
//	POINT	_ptDest;
//
//public:
//	SbrXTrj_Sin( const PPOINT pt, int to_x, int to_y, float freq, float amp, float phase = 0.0f );
//
//	bool	Step( PPOINT pt, float fTimeDelta );
//	void	MoveTo( int x, int y );
//	void	SetSpeed( const PPOINT pt, float fSpeed );
//};

//-------------------------------------------------------------

#endif