//-------------------------------------------------------------
// Nebula Game Core player class declaration file
//-------------------------------------------------------------

#include "../Saber Graphic Engine/SaberHUDClass.h"

#include "GameCoreClass_Ships.h"
#include "GameCoreClass_Misc.h"

#ifndef _GAME_CORE_PLAYER_
#define _GAME_CORE_PLAYER_

//-------------------------------------------------------------

enum PlayerState
{
	Alive		= 1,
	Spawning	= 2,
	Spawned		= 3,
	Looser		= 4
};

//-------------------------------------------------------------
// Main class declaration
//-------------------------------------------------------------

/**
 * Class which represents player with its data such as lifes, ammo, score.
 * I feel really sorry for this class, because it mixes <<entity>> and <<control>> responsibilities.
 * Also it uses enum and has switch operator inside its method. It's hopeless.
 */
class Player
{
protected:
	PlayerShip*			_pShip;				/**< Pointer to PlayerShip object which represents ship controlled by this player. */

	SbrHUDCounter*		_pLives;			/**< Pointer to interface lifes counter. */
	SbrHUDCounter*		_pScore;			/**< Pointer to interface score counter. */
	SbrHUDCounter*		_pAmmo;				/**< Pointer to interface ammo counter. */
	SbrSprite*			_pLivesSpr;			/**< Pointer to sprite which displayed near player' lifes counter. */
	SbrSprite*			_pAmmoSpr;			/**< Pointer to sprite which displayed near player' ammo counter. */

	Combo*				_pCombo;			/**< Pointer to Combo class instance for current player. */

	PlayerState			_eState;			/**< Current player state. */
	INT					_nNextCap;			/**< Amount of score points */
	FLOAT				_fCooldown;			/**< Cooldown for current player state. */
	BOOL				_bLock;				/**< True if player are locked for now and can't interract with game world, otherwise false. */

public:
	/**
	 * Creates player with specified number.
	 */
	Player();
	/**
	 * Destructor.
	 */
	~Player();

	/**
	 * Allows player to process its state and ask its ship: "Do u shoot any aliens? ;)"
	 * @param fTimeDelta time in msc's since last call to this method.
	 */
	void				Process( float fTimeDelta );
	/**
	 * Allows player to render counters such as life, ammo and score counter and their sprites.
	 */
	virtual		void	RenderCounters();
	/**
	 * Not implemented yet.
	 */
	void				ShowStatistics();

	/**
	 * Shoots with main weapon of player ship.
	 */
	void				Shoot();
	/**
	 * Shoots with rocket (if there are any of them left).
	 */
	void				ShootRocket();
	/**
	 * Starts moving to most left position of the player screen.
	 */
	void				StrafeLeft() const { if ( !_bLock ) _pShip->StrafeLeft(); }
	/**
	 * Starts moving to most right position of the player screen. Isn't it obvious?
	 */
	void				StrafeRight() const { if ( !_bLock ) _pShip->StrafeRight(); }
	/**
	 * Stops player ship from moving.
	 */
	void				Stop() const { if ( !_bLock ) _pShip->Stop(); }
	/**
	 * Returns player ship to its start position according on the player' number.
	 */
	virtual		void	ResetPos();
	/**
	 * Sets shield on player ship.
	 */
	virtual		void	Shield();
	/**
	 * Removes shield from player ship.
	 */
	virtual		void	Unshield();
	/**
	 * Save current player game statistics to a text file in game directory.
	 */	
	virtual		bool	SaveStat() const;
	/**
	 * Returns current player state.
	 */
	PlayerState			GetState() const { return _eState; }
	
	/**
	 * Locks Player so it couldn't interract with game world anymore.
	 */
	void				Lock() { Stop(); _bLock = true; }
	/**
	 * Unlocks Player.
	 */
	void				Unlock() { _bLock = false; } 
};

//-------------------------------------------------------------

/**
 * Class which represents player one in multiplayer game.
 */
class PlayerOne: public Player
{
public:
	/**
	 * Player one specified contructor.
	 */
	PlayerOne();

	void ResetPos();
	/**
	 * Sets shield on player one ship.
	 */
	void	Shield();
	/**
	 * Removes shield from player one ship.
	 */
	void	Unshield();
	/**
	 * Save current player game statistics to a text file in game directory.
	 */	
	bool	SaveStat() const;
};

//-------------------------------------------------------------

/**
 * Class which represents player two in multiplayer game.
 */
class PlayerTwo: public Player
{
public:
	/**
	 * Player two specified contructor.
	 */
	PlayerTwo();

	void ResetPos();
	/**
	 * Sets shield on player two ship.
	 */
	void	Shield();
	/**
	 * Removes shield from player two ship.
	 */
	void	Unshield();
	/**
	 * Allows player to render counters such as life, ammo and score counter and their sprites.
	 */
	void	RenderCounters();
	/**
	 * Save current player game statistics to a text file in game directory.
	 */	
	bool	SaveStat() const;
};

//-------------------------------------------------------------

//-------------------------------------------------------------

#endif