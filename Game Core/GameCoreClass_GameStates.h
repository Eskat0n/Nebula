//-------------------------------------------------------------

#ifndef _GAME_STATES_
#define _GAME_STATES_

//-------------------------------------------------------------
// Forward declarations
//-------------------------------------------------------------

class Game;

//-------------------------------------------------------------

/**
 * Game state interface.
 */
class IGameState
{
public:
	/**
	 * Polymorphic method which handles current state behavior.
	 * @param game pointer to game instance for which this state are valid.
	 * @param fTimeDelta time elapsed since last call to this method.
	 */
	virtual	IGameState*	Process( Game* const game, float fTimeDelta ) = 0;
};

//-------------------------------------------------------------

/**
 * Implementation of State_LevelStarting which represents "level are starting" case.
 */
class State_LevelStarting: public IGameState
{
private:
	float	_fDuration;			/**< Duration of current state. */

public:
	/**
	 * Constructor.
	 * @param game pointer to game instance for which this state are valid.
	 */
	State_LevelStarting( Game* const game );
	virtual	IGameState*	Process( Game* const game, float fTimeDelta );
};

//-------------------------------------------------------------

/**
 * Implementation of State_InGame which represents "game in process" case.
 */
class State_InGame: public IGameState
{
public:
	/**
	 * Contructor.
	 * @param game pointer to game instance for which this state are valid.
	 */
	State_InGame( Game* const game );
	virtual	IGameState*	Process( Game* const game, float fTimeDelta );
};

//-------------------------------------------------------------

/**
 * Implementation of State_CompletingLevel which represents "level are completed" case.
 */
class State_CompletingLevel: public IGameState
{
private:
	float	_fDuration;			/**< Duration of current state. */

public:
	/**
	 * Constructor.
	 * @param game pointer to game instance for which this state are valid.
	 */
	State_CompletingLevel( Game* const game );
	virtual	IGameState*	Process( Game* const game, float fTimeDelta );
};

//-------------------------------------------------------------

/**
 * Implementation of State_GameComplete which represents "game are complete" case.
 */
class State_GameComplete: public IGameState
{
private:
	float	_fDuration;			/**< Duration of current state. */

public:
	/**
	 * Constructor.
	 * @param game pointer to game instance for which this state are valid.
	 */
	State_GameComplete( Game* const game );
	virtual	IGameState*	Process( Game* const game, float fTimeDelta );
};

//-------------------------------------------------------------

/**
 * Implementation of State_GameOver which represents "game over" case.
 */
class State_GameOver: public IGameState
{
private:
	float	_fDuration;			/**< Duration of current state. */

public:
	/**
	 * Constructor.
	 * @param game pointer to game instance for which this state are valid.
	 */
	State_GameOver( Game* const game );
	virtual	IGameState*	Process( Game* const game, float fTimeDelta );
};

//-------------------------------------------------------------

/**
 * Implementation of State_GameOver which represents "game over" case.
 */
class State_Credits: public IGameState
{
private:
	float	_fDuration;			/**< Duration of current state. */

public:
	/**
	 * Constructor.
	 * @param game pointer to game instance for which this state are valid.
	 */
	State_Credits( Game* const game );
	virtual IGameState* Process( Game* const game, float fTimeDelta );
};

//-------------------------------------------------------------

#endif