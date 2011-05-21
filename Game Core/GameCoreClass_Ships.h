//-------------------------------------------------------------
// Nebula Game Core ship classes declaration file
//-------------------------------------------------------------

#include "../SaberX Engine/SaberXEngineClass.h"

#include "GameDeclarations.h"

#ifndef _GAME_CORE_SHIPS_
#define _GAME_CORE_SHIPS_

//-------------------------------------------------------------
// Forward declarations
//-------------------------------------------------------------

class Ship;

//-------------------------------------------------------------
// Classes declarations
//-------------------------------------------------------------

/**
 * Class which represents a game weapon and shots it fires.
 * I am so lazy to write weapons classes wich can produce shot instances (and implements factory pattern).
 * Also there are no object which inherit from Shot class so shots differs only by its id, speed, damage etc.
 * This is really horrible and I should rewrite it ASAP.
 */
class Shot: public MovingObject
{
private:
	SbrAnim*	_pShotEffect;						/**< Effect which appears on the screen when shot are fired. Not used yet. */
	SbrAnim*	_pHitEffect;						/**< Effect which appears on the screen when shot are hits its target. Not used yet. */
	INT			_nDmg;								/**< Damage amount dealing by shot. */
	Ship*		_pOwner;							/**< Pointer to the ship which fired this shot. */

public:
	/**
	 * Constructor.
	 * @param id ID of created shot instance.
	 * @param spr pointer to sprite which represents shot on the screen.
	 * @param dmg damage of shot.
	 * @param speed speed of shot (in px/sec).
	 * @param owner pointer to Ship class instance which owns newly created shot.
	 */
	Shot( int id, SbrSprite* spr, int dmg, int speed, Ship* owner );
	/**
	 * Destructor.
	 */
 	virtual		~Shot() { };

	/**
	 * Allows shot to process its state.
	 * @param fTimeDelta time in msc's since last call to this method.
	 */
	virtual		void	Process( float fTimeDelta );
	/**
	 * Handles collision with other objects.
	 * This method contains VERY horrible switch-by-state operator but it still working.
	 * @param obj pointer to object class instance this object is collided with.
	 */
	virtual		void	OnCollide( Object* obj );
	/**
	 * Shoots this shot from specified position by specified angle.
	 * @param x coordinate of position shot will be shooted from.
	 * @param y coordinate of position shot will be shooted from.
	 * @param ang angle in radians. Use DTRAD macro to in-place conversion from degrees to radians.
	 */
				void	Shoot( int x, int y, float ang );
	/**
	 * Shoots this shot from specified position to specified position.
	 * @param x coordinate of position shot will be shooted from.
	 * @param y coordinate of position shot will be shooted from.
	 * @param mvx coordinate of position shot will be shooted to.
	 * @param mvy coordinate of position shot will be shooted to.
	 */
				void	Shoot( int x, int y, int mvx, int mvy );
	/**
	 * Return pointer to Ship which owns this shot.
	 * @return pointer to Ship class instance.
	 */
				Ship*	GetOwner() const { return _pOwner; }
	/**
	 * Returns amount of damage dealing by this shot.
	 * @return damage amount of damage.
	 */
				int		GetDamage() const { return _nDmg; }
};

//-------------------------------------------------------------

/**
 * Abstract concept of any ship in game. 
 * It was first class I coded among all GameCore classes. 
 */
class Ship: public MovingObject
{
protected:
	/**
	 * Creates ship with specified max armor.
	 * @param maxarm max amount of armor for this ship.
	 */
	Ship( int maxarm );
	/**
	 * Destructor.
	 */
	virtual		~Ship();

	INT			_nMaxArmor;				/**< Max amount of armor for this ship. */
	INT			_nCurArmor;				/**< Current amount of armor for this ship. */
	BOOL		_bDestroyable;			/**< True if this ship can be destroyed, otherwise false. Do you want to make local Chuck Norris? - just set this var to false. */
	INT			_nCost;					/**< Cost of this ship in game score points. */
	FLOAT		_fShipSpeed;			/**< Linear speed of this ship. */
	Effect*		_pDestroyEffect;		/**< Effect which appears on the screen when this ship are destroyed. s*/

public:
	/**
	 * Destroys ship and shows specified effect (if there are any).
	 */
	virtual		void	Destroy();
	/**
	 * Repairs ships.
	 * Restores ship' current armor to its max value and shows ship.
	 */
				void	Repair() { Show(); _nCurArmor = _nMaxArmor; }
	/**
	 * Sets destroyability for this ship.
	 * @param value true if ship will be destroyable, otherwise false.
	 */
				void	Destroyable( bool value = true ) { _bDestroyable = value; }
	/**
	 * Hits ship for certain amount of damage.
	 * If ship' current armor after getting hitted falls below zero - ship would be destroyed.
	 * @param damage amount of damage ship will receive.
	 * @return false if ship are destoryed after getting hitted, true if alive.
	 */
				bool	Hit( int damage );
	/** 
	 * Returns current amount of armor of the ship.
	 * @return current armor.
	 */
				int		GetConsistency() const { return _nCurArmor; }
	/**
	 * Increases this ship cost by specified value.
	 * @param cost amount of score points ship' cost will be increased by.
	 */
				void	AddCost( int cost ) { _nCost += cost; }
	/**
	 * Returns cost of the ship.
	 * @return cost of the ship.
	 */
				int		GetCost() { return _nCost; }
	/**
	 * Changes color of ship.
	 * Color will be change for all corners of sprite' quad.
	 * @param color 32-bit color value.
	 */
				void	ChangeColor( DWORD color ) { if ( _pSpr ) _pSpr->SetColor( color ); }
};

//-------------------------------------------------------------

/**
 * Player ship class.
 * Represents fighter ship controlled by player.
 * It have internal reload parameters, because I am so lazy to implement standalone weapon classes.
 */
class PlayerShip: public Ship
{
private:
	BOOL		_bMainWpnReload;		/**< True if main weapon (mgun) are currently reloading, otherwise false. */
	BOOL		_bRocketsReload;		/**< True if rocket launcher are currently reloading, otherwise false. */
	FLOAT		_fMainRelTime;			/**< Time left for main weapon to reload. */
	FLOAT		_fRocketsRelTime;		/**< Time left for rocket launcher to reload. */
	Shot*		_pMainWpn[3];			/**< Pointers to Shots used by main weapon. 3 shots are sufficient. */
	INT			_nCurMainShot;			/**< Index of shot to be shooted next. */
	INT			_nRocketsCount;			/**< Quantity of rockets left. */
	Shot*		_pRockets;				/**< Pointer to Shot wich represents missiles. */

public:
	/**
	 * Constructor.
	 */
	PlayerShip();
	/**
	 * Destructor.
	 */
	virtual		~PlayerShip();

	/**
	 * Allows player ship to process its state.
	 * @param fTimeDelta time in msc's since last call to this method.
	 */
	virtual		void	Process( float fTimeDelta );
	/**
	 * Handles collision with other objects.
	 * @param obj pointer to object class instance this object is collided with.
	 */
	virtual		void	OnCollide( Object* obj );
	/**
	 * Returns current player ship cost and nulifies its value after each call.
	 * @return player ship cost.
	 */
	virtual		int		GetCost();
	/**
	 * Starts moving to most left position of the player screen.
	 */
				void	StrafeLeft();
	/**
	 * Starts moving to most right position of the player screen. Isn't it obvious?
	 */
				void	StrafeRight();
	/**
	 * Shoots with main weapon.
	 */
				void	Shoot();
	/**
	 * Shoots with rocket (if there are any of them left).
	 */
				void	ShootRocket();
	/**
	 * Returns amount of rockets left.
	 * @return rockets left.
	 */
				int		GetRockets() const { return _nRocketsCount; }
};

//-------------------------------------------------------------

/**
 * Class which brings concept of abstract enemy ship.
 * It's inherited from Ship, so it can fly through world, but can be grouped into Armade - solid fleet of enemy ships.
 * Each enemy ship is able to have any kind of special ability. 
 * Ability are unique, so it are need to be coded directly into UseAbility method after inheriting from this class.
 */
class EnemyShip: public Ship
{
protected:
	POINT		_ptArmadePos;				/**< Position in armade. */
	Shot*		_pWpn;						/**< Pointer to Shot class instance which represents type of shot fired by this ship. */
	FLOAT		_fShootChance;				/**< Shoot chance (in percents) for ship. */
	FLOAT		_fAbilityChance;			/**< Chance of use*/
	BOOL		_bReload;					/**< True if this ship main weapon are currently reloading. */
	BOOL		_bAbilityReload;			/**< True if this ship ability isn't ready yet. */
	FLOAT		_fWpnReloadTime;			/**< Time left for main weapon to reload. */
	FLOAT		_fAbReloadTime;				/**< Time left for ability to prepare. */
	BOOL		_bIndependent;				/**< True if this ship are indepentent from Armade behavior it belongs to. */

public:
	/**
	 * Creates enemy ship with specified parameters.
	 * @param id ID of newly created ship.
	 * @param spr pointer to sprite used to represent this ship on the screen.
	 * @param maxarm max armor for newly created ship.
	 * @param arX horizontal coordinate of Armade position. 
	 * @param arY vertical coordinate of Armade position.
	 */
	EnemyShip( int id, SbrSprite* spr, int maxarm, int arX, int arY );
	/**
	 * Destructor.
	 */
	virtual		~EnemyShip();

	/**
	 * Shoots by default weapon if chance check is succeeded.
	 * @param forSure if set to true chances check and 'dices' throw will be skipped. 
	 */
	virtual		void	Shoot( bool forSure = false ) = 0;
	/**
	 * Uses ability if chance check is succeeded.
	 * @param forSure if set to true chances check and 'dices' throw will be skipped. 
	 */
	virtual		void	UseAbility( bool forSure = false ) = 0;
	/**
	 * Starts moving to most left position in Armade.
	 */
	virtual		void	StrafeLeft();
	/**
	 * Starts moving to most right position in Armade.
	 */
	virtual		void	StrafeRight();
	/**
	 * Method derived from Ship::Destroy.
	 * In addition to destroying separates ship from armade.
	 */
				void	Destroy();
	/**
	 * Perfroms check for _bIndependent field to determine ship are still among Armade lines or not.
	 * @return true if ship are still in Armade, otherwise false.
	 */
				bool	IsInArmade() const { return !_bIndependent; }
};

//-------------------------------------------------------------

/**
 * 1st enemy ship type - Pod class.
 * Ship name is... oh, I forget how I was think up it. 
 */
class Pod: public EnemyShip
{
public:
	/**
	 * Creates pod with specified parameters.
	 * @param spr pointer to pod sprite.
	 * @param arX horizontal coordinate of Armade position. 
	 * @param arY vertical coordinate of Armade position.
	 */
	Pod( SbrSprite* spr, int arX, int arY );
	/**
	 * Destructor.
	 */
	virtual		~Pod() {}

	/**
	 * Derived from EnemyShip::Process. Description are same.
	 */
	virtual		void	Process( float fTimeDelta );
	/**
	 * Derived from EnemyShip::Shoot. Description are same.
	 */
	virtual		void	Shoot( bool forSure = false );
	/**
	 * Derived from EnemyShip::UseAbility. Description are same.
	 * Borg ability is kamikadze.
	 */
	virtual		void	UseAbility( bool forSure = false );
	/**
	 * Derived from EnemyShip::OnCollide. Description are same.
	 */
	virtual		void	OnCollide( Object* obj );
};

//-------------------------------------------------------------

/**
 * 2st enemy ship type - Borg class.
 * Ship name is stealed from StarTrek series.
 */
class Borg: public EnemyShip
{
public:
	/**
	 * Creates borg with specified parameters.
	 * @param spr pointer to pod sprite.
	 * @param arX horizontal coordinate of Armade position. 
	 * @param arY vertical coordinate of Armade position.
	 */
	Borg( SbrSprite* spr, int arX, int arY );
	/**
	 * Destructor.
	 */
	virtual		~Borg() {}

	/**
	 * Derived from EnemyShip::Process. Description are same.
	 */
	virtual		void	Process( float fTimeDelta );
	/**
	 * Derived from EnemyShip::Shoot. Description are same.
	 */
	virtual		void	Shoot( bool forSure = false );
	/**
	 * Derived from EnemyShip::UseAbility. Description are same.
	 * Borg ability is temporary invisibility.
	 */
	virtual		void	UseAbility( bool forSure = false );
	/**
	 * Derived from EnemyShip::OnCollide. Description are same.
	 */
	virtual		void	OnCollide( Object* obj );
};

//-------------------------------------------------------------

/**
 * 2st enemy ship type - Tres class.
 * Ship name is derived from android name from Trinity Blood anime series.
 */
class Tres: public EnemyShip
{
private:
	Shot*		_pAbWpn[3];				/**< Pointer to shots used by triple shot ability. */

public:
	/**
	 * Creates tres with specified parameters.
	 * @param spr pointer to pod sprite.
	 * @param arX horizontal coordinate of Armade position. 
	 * @param arY vertical coordinate of Armade position.
	 */
	Tres( SbrSprite* spr, int arX, int arY );
	/**
	 * Destructor.
	 */
	virtual		~Tres();

	/**
	 * Derived from EnemyShip::Process. Description are same.
	 */
	virtual		void	Process( float fTimeDelta );
	/**
	 * Derived from EnemyShip::Shoot. Description are same.
	 */
	virtual		void	Shoot( bool forSure = false );
	/**
	 * Derived from EnemyShip::UseAbility. Description are same.
	 * Borg ability is triple shot.
	 */
	virtual		void	UseAbility( bool forSure = false );
	/**
	 * Derived from EnemyShip::OnCollide. Description are same.
	 */
	virtual		void	OnCollide( Object* obj );
};

//-------------------------------------------------------------

/**
 * 2st enemy ship type - Gungir class.
 * Ship name is derived Odin's spear which rumored to never miss its target.
 */
class Gungir: public EnemyShip
{
public:
	/**
	 * Creates gungir with specified parameters.
	 * @param spr pointer to pod sprite.
	 * @param arX horizontal coordinate of Armade position. 
	 * @param arY vertical coordinate of Armade position.
	 */
	Gungir( SbrSprite* spr, int arX, int arY );
	/**
	 * Destructor.
	 */
	virtual		~Gungir() {}

	/**
	 * Derived from EnemyShip::Process. Description are same.
	 */
	virtual		void	Process( float fTimeDelta );
	/**
	 * Derived from EnemyShip::Shoot. Description are same.
	 */
	virtual		void	Shoot( bool forSure = false );
	/**
	 * Derived from EnemyShip::UseAbility. Description are same.
	 * Gungirs have no abilities.
	 */
	virtual		void	UseAbility( bool forSure = false );
	/**
	 * Derived from EnemyShip::OnCollide. Description are same.
	 */
	virtual		void	OnCollide( Object* obj );
};

//-------------------------------------------------------------

#endif