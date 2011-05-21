//-------------------------------------------------------------
// Nebula Game Core Armade class declaration file
//-------------------------------------------------------------

#include "GameCoreClass_Ships.h"
#include "GameCoreClass_Misc.h"
#include "GameDeclarations.h"

#ifndef _GAME_CORE_ARMADE_
#define _GAME_CORE_ARMADE_

//-------------------------------------------------------------

/**
 *	Class which holds enemy ships and process their movements.
 */
class Armade 
{
private:
	EnemyShip*				_pShips[4][10];					/**< Array of pointer to enemy ships. */
	EnemyShip*				_pControlShip;					/**< Control ship used in movement direction changing cases. */
	INT						_nShipsCount;					/**< Number of ships currently alive in Armade. */
	BOOL					_bMoveLeft;						/**< True if Armade are currently moving left. */
	BOOL					_bMoving;						/**< Ture if Armade are currently moving. */
	BOOL					_bLock;							/**< True if Armade are locked now. */

public:
	/**
	 * Contructor.
	 * @param boss if true Armade will be filled with one boss ship.
	 */
	Armade( bool boss = false );
	/**
	 * Destructor.
	 */
	~Armade();

	/**
	 * Allow Armade to process it's state and current movement.
	 * @param fTimeDelta amount of time elapsed from last processing.
	 */
	void					Process( float fTimeDelta );

	/**
	 * Returns number of alive ships.
	 * @return number of alive ships.
	 */
	int						GetShipsCount() { return _nShipsCount; }

	/**
	 * Locks Armade to prevent any change of it's state by outer influences.
	 */
	void					Lock() { _bLock = true; }
	/**
	 * Unlocks Armade for further use.
	 */
	void					Unlock() { _bLock = false; } 
};

//-------------------------------------------------------------

#endif