//-------------------------------------------------------------
// Nebula Game Core misc classes declaration file
//-------------------------------------------------------------

#include "../Saber Graphic Engine/SaberHUDClass.h"

#include "GameCoreClass_Ships.h"

#ifndef _GAME_CORE_MISC_
#define _GAME_CORE_MISC_

//-------------------------------------------------------------

/**
 * Class which represents player' combo. 
 * So its quake-style messages are indeed stealed from Quake III: Arena, not from UT.s
 */
class Combo 
{
private:
	SbrHUDMessage*	_pMessage;						/**< The message which appears on the screen when combo upgrading. */

	INT				_nCurLevel;						/**< Current level of combo. 0 represents no-combo and 5 is max leve. */
	FLOAT			_fTimeElapsed;					/**< Time elapsed since last frag (in msc). */

public:
	/**
	 * Creates combo for specified player.
	 * Player nubmer affects position on the screen in which combo message appears.
	 * @param No number of player (0 for 1st player, otherwise combo will be created for 2nd player).
	 */
	Combo( int No );
	/**
	 * Combo destructor.
	 */
	~Combo();

	/**
	 * Allow combo to process its status.
	 * Ticks internal time and perform self-reset if certain amount of time are elapsed.
	 * @param fTimeDelta time in msc's since last call to this method.
	 */
	void			Process( float fTimeDelta );
	/**
	 * Increases level of the combo.
	 */
	void			Increase();
	/**
	 * Allow combo to render its message.
	 */
	void			Render();

	/**
	 * Returns ratio used in caounting player' score.
	 * @return ratio dependent on combo' level.
	 */
	float			GetRatio() const;
};

//-------------------------------------------------------------

/**
 * Class which represents mega-blaster used by enemy cruisers.
 * Inherited from StaticObject since it isn't shootable. 
 * This is actually isn't MegaBlaster itseft, as it can be cosidered as a weapon -
 * it's just a mega blaster beam with unique id (and accordingly infinite destroy power).
 * Mega blaster beam just appears on the screen and collides with players ships destroying them.
 */
class MegaBlaster: public StaticObject
{
private:
	BOOL			_bFadeIn;							/**< True if mega blaster beam are currently fading in, if false beam are fading out. */
	FLOAT			_fFadeTime;							/**< Time remained for beam to completely appear/dissappear from screen. */

public:
	/** 
	 * Default constructor.
	 */
	MegaBlaster();
	/** 
	 * Destructor.
	 */
	~MegaBlaster();

	/**
	 * Allows mega blaster to process its state and perform needed alpha-channel calculations.
	 * @param fTimeDelta time in msc's since last call to this method.
	 */
	virtual		void	Process( float fTimeDelta );
	/**
	 * Handles collision with other objects.
	 * @param obj pointer to object class instance this object is collided with.
	 */
	virtual		void	OnCollide( Object* obj );
	/**
	 * Places mega blaster beam in specified position and it starts to fade in.
	 * @param x coordinate x of screen position.
	 * @param y coordinate y of screen position.
	 */
				void	Blast( int x, int y );
	/**
	 * Updates mega blaster beam position so it can follow its source.
	 * @param x coordinate x of screen position beam will be replaced to.
	 * @param y coordinate y of screen position beam will be replaced to. 
	 */
				void	Update( int x, int y );
	/**
	 * Slowly clears mega blaster beam from screen with fade out effect.
	 */
				void	Disappear();
};

//-------------------------------------------------------------

/**
 * Boss ship class (actually a concrete cruiser type - "Gaia").
 * Inherited from EnemyShip so you can fill the entire Armade with this cruisers. Isn't sounds cool?
 */
class BossShip: public EnemyShip
{
private:
	MegaBlaster*	_pMegaBlaster;							/**< Pointer to MegaBlaster instance used by this cruiser. */
	Shot*			_pAddWpns[3];							/**< Pointers to additional Shot instances so BossShip can fire 4 blasts in essence. */
	FLOAT			_fMBCooldown;							/**< Cooldown between two MegaBlaster shots. */
	
public:
	/**
	 * Default constructor.
	 */
	BossShip();
	/**
	 * Destructor.
	 */
	~BossShip();

	/**
	 * Allows boss ship to process its state.
	 * @param fTimeDelta time in msc's since last call to this method.
	 */
	virtual		void	Process( float fTimeDelta );
	/**
	 * Shoots by default weapon if chance check is succeeded.
	 * @param forSure if set to true chances check and 'dices' throw will be skipped. 
	 */
	virtual		void	Shoot( bool forSure = false );
	/**
	 * Uses ability if chance check is succeeded (fires the mega blaster).
	 * See EnemyShip::UseAbility for details.
	 * @param forSure if set to true chances check and 'dices' throw will be skipped. 
	 */
	virtual		void	UseAbility( bool forSure = false );
	/**
	 * Handles collision with other objects.
	 * @param obj pointer to object class instance this object is collided with.
	 */
	virtual		void	OnCollide( Object* obj );
	/**
	 * Starts moving to most left position in Armade.
	 */
	virtual		void	StrafeLeft();
	/**
	 * Starts moving to most right position in Armade.
	 */
	virtual		void	StrafeRight();
};

//-------------------------------------------------------------

#endif
