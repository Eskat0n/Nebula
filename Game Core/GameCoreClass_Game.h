//-------------------------------------------------------------

#include "../Azumaria Sound Engine/AzuEngineClass.h"
#include "../Saber Graphic Engine/SaberGE_Dispatcher.h"
#include "../SaberX Engine/SbrXFramerClass.h"
#include "../SaberX Engine/SaberXEngineClass.h"

#include "GameCoreClass_Armade.h"
#include "GameCoreClass_Player.h"
#include "GameCoreClass_Ships.h"
#include "GameCoreClass_GameStates.h"

#ifndef _GAME_CORE_MAIN_
#define _GAME_CORE_MAIN_

//-------------------------------------------------------------

enum GameType
{
	SINGLEPLAY		= 1,
	HOTSEAT			= 2,
	MULTIPLAYER		= 3
};

//-------------------------------------------------------------

/**
 * Main game class.
 * This class resembles implementation of Nebula game logic which uses tools provided by SaberX Engine.
 * It haven't any polymorphic method so it's really horrible.
 */
class Game
{
private:
	static	Game*	_self;				/**< pointer to only instance of this class. */
	static	int		_nRefCount;			/**< number of references created to only instance of this class. */
								

	/**
	 * Constructor.
	 * @type game type to create.
	 */
	Game( GameType type );
	/** 
	 * Destructor.
	 */
	~Game();

private:
	GameType		_eType;				/**< Type of game currently running. */
	IGameState*		_pState;			/**< Pointer to IGameState implementation which resembles current game state. */

	ISaberGE*		_pSaber;			/**< Pointer to instance of SaberGE Engine guarding proxy. */
	AzuEngine*		_pAzuEngine;		/**< Pointer to instance of Azumaria Sound Engine. */
	SbrSprite*		_pBackground;		/**< Pointer to sprite which contains background image. */

	SbrXFramer*		_pFramer;			/**< Pointer to instance of SaberX Framer subsystem. */
	World*			_pWorld;			/**< Pointer to instance of World metasystem. */

	Player*			_pPlayer[2];		/**< Pointers to player one object and player two object (if needed). */
	Armade*			_pArmade;			/**< Pointer to Armade class instance. */
	SbrHUDMessage*	_pMessage;			/**< Pointer to message which appears on screen. */

	INT				_nCurLevel;			/**< Current level are currently in game. */
	BOOL			_bRunning;			/**< True if game are running now, otherwise false. */
	BOOL			_bSuspend;			/**< True if game are currently suspended, otherwise false. */

	/** Static function to be loaded into SaberX Framer as glogic_func. */
	static	bool	GameLogic();
	/** Static function to be loaded into SaberX Framer as render_func. */
	static	bool	RenderLogic();
	/** Static function to be loaded into SaberGE as on_close_func. */
	static	void	OnCloseOfGame();

public:
	static	Game*	Instance( GameType type );
	void			Release();

	/**
	 * Starts game with current type.
	 */
	void			Begin();
	/**
	 * Ends game.
	 */
	void			End();

	/**
	 * Suspends game.
	 * Stops processing of all game-incapsulated object as well as user input and back buffer presenting.
	 */
	static	void	Suspend();
	/**
	 * Continues game.
	 */
	static	void	Continue();
	bool			Process();

	/**
	 * Check input queue for key which cause game interrupting (such as Esc, Pause etc.). 
	 */
	bool			CheckControl();
	/**
	 * Dispatch controls from input queue.
	 */
	bool			DispatchControls();
	/**
	 * Changes level and destroys currently destroyed Armade and creates new.
	 */
	bool			ChangeLevel();

	/**
	 * Returns pointer to World instance.
	 */
	World* const			GetWorld() const { return _pWorld; }
	/**
	 * Returns pointer to specified player.
	 * @param n index of player to return.
	 * @return pointer to specified player: player one if n = 0, otherwise player two.
	 */
	Player* const			GetPlayer( int n ) const { return _pPlayer[ (n == 0) ? 0:1 ]; }
	/**
	 * Returns pointer to Armade.
	 * @return pointer to Armade.
	 */
	Armade* const			GetArmade() const { return _pArmade; }
	/**
	 * Returns pointer to text message used in game.
	 * @return pointer to message object.
	 */
	SbrHUDMessage* const	GetMsg() const { return _pMessage; }
	/**
	 * Returns number of current level (mission in game terms).
	 * @return number of current level.
	 */
	int						GetCurLevel() const { return _nCurLevel; }

	/**
	 * Return game type currently running.
	 * @return type of game currently running.
	 */
	GameType		GetType() const { return _eType; }
};

//-------------------------------------------------------------

//-------------------------------------------------------------

#endif