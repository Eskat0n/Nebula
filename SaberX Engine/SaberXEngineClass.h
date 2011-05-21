//-------------------------------------------------------------

#include "../Saber Graphic Engine/SaberGE_Animation.h"
#include "SaberXTrajectoriesDecl.h"

#include <vector>

#ifndef _SABERX_ENGINE_
#define _SABERX_ENGINE_

//-------------------------------------------------------------
// Forward declarations
//-------------------------------------------------------------

class World;

//-------------------------------------------------------------
// Other definitions
//-------------------------------------------------------------

enum BND_REL
{
	SBRX_BND_IGNORE	= 0,
	SBRX_BND_MIRROR = 1,
	SBRX_BND_BOUNCE = 2
};

//-------------------------------------------------------------
// Object classes definitions
//-------------------------------------------------------------

/**
 * Abstact concept of any game world' object which can express its properties via collisions with other objects.
 */
class Object
{
protected:
	/**
	 * Default constructor.
	 */
	Object();

	SbrSprite*		_pSpr;				/**< Pointer to sprite which represents this object on the screen. */
	
	INT				_nID;				/**< ID of this object (used for some unfair tricks). */
	POINT			_ptPos;				/**< Position of this object in logical coordinates. */
	POINT			_ptHotSpot;			/**< Coordinates of hotspot point of this object. */
	RECT			_rcHitbox;			/**< Hitbox of this object (used in collision detection). */
	BOOL			_bVisible;			/**< True if object is visible and can be rendered, otherwise false. */

private:
	World*	_pWorld;					/**< Pointer to world class instance this object belongs to. */

public:
	/**
	 * Constructor. 
	 * Automatically creates world object instance (if there are no any) and link newly created object to world.
	 * @param x starting position x coordinate.
	 * @param y starting position y coordinate.
	 */
	Object( int id, int x = 0, int y = 0 );
	virtual		~Object();

	/**
	 * Allows object to process its state.
	 * Is pure virtual method since object is abstraction at all.
	 * @param fTimeDelta time in msc's since last call to this method.
	 */
	virtual		void		Process( FLOAT fTimeDelta ) = 0;
	/**
	 * Checks this object for collision with other object.
	 * @param obj pointer to object which possibly collides with this object.
	 * @return true if collision happened, otherwise false.
	 */
				bool		Collide( Object* obj );
	/**
	 * Handles collision with other objects.
	 * Is pure virtual method since object is abstraction at all.
	 * @param obj pointer to object class instance this object is collided with.
	 */
	virtual		void		OnCollide( Object* obj ) = 0;
	/**
	 * Renders sprite associated with this object on the screen.
	 */
	virtual		void		Render();

	/**
	 * Allows rendering of the object on the screen.
	 */
				void		Show(){ _bVisible = true; }
	/**
	 * Forbids rendering of the object on the screen.
	 */
				void		Hide() { _bVisible = false; }

	/** 
	 * Sets position for object.
	 * NOTE: Position will be setted to its hot-spot point.
	 */
				void		SetPos( int x, int y ) { _ptPos.x = x; _ptPos.y = y; }
	/**
	 * Sets new sprite for this object.
	 * @param spr pointer to new sprite for this object.
	 */
				void		SetSprite( SbrSprite* spr );
	/**
	 * Sets hitbox for this object.
	 * @param rect pointer to RECT struct which contains hitbox dimessions (in object' left-upper corner relative coordinates).
	 */
				void		SetHitbox( RECT* rect );
	/**
	 * Sets hitbox for this object.
	 * @param x left-upper hitbox corner x coordinate.
	 * @param y left-upper hitbox corner y coordinate.
	 * @param w width of the hitbox.
	 * @param h height of the hitbox.
	 */
				void		SetHitbox( int x, int y, int w, int h );
	/** 
	 * Sets hotspot point position of the object.
	 * NOTE: Coordinates must be set in object' upper-left corner relative coordinate system.
	 * @param x hotspot coordinate along x axis.
	 * @param y hotspot coordinate along y axis.
	 */
				void		SetHotSpot( int x, int y );

	/** 
	 * Returns id of current object.
	 * @return object' ID
	 */
				int			GetID() { return _nID; }
	/**
	 * Returns hot spot point x coordinate in global logical coordinates.
	 * @return coordinate along x axis.
	 */
				int			GetPosX() { return _ptPos.x; }
	/**
	 * Returns hot spot point y coordinate in global logical coordinates.
	 * @return coordinate along y axis.
	 */
				int			GetPosY() { return _ptPos.y; }
	/**
	 * Returns pointer to hitbox of this object.
	 * @return pointer to RECT struct.
	 */
				RECT*		GetHitbox() { return &_rcHitbox; }
	/**
	 * Return visibility of this object.
	 * @return true if this object can be rendered on the screen, otherwise false.
	 */
				bool		IsVisible() { return _bVisible; }
};

//-------------------------------------------------------------

/**
 * Further revision of the abstract object.
 * Moving object is any game world' object which can move with predefined trajectory from point to point.
 * Developing of this class was really long as development of all GameCore classes itself.
 */ 
class MovingObject: public Object
{
private:
	PMV_TRJ			_pMovTrj;				/**< Pointer to MV_TRJ class instance which handles this object movement. */
	FLOAT			_fSpeed;				/**< Speed (in px/sec) this object is moving with. */
	FLOAT			_fFriction;				/**< Not used yet. */
	BND_REL			_boundary;				/**< Relations with world's boundaries (not used yet). */

	BOOL			_bMoving;				/**< True if object are currently moving, otherwise false. */

public:
	/**
	 * Creates moving object with specified parameters.
	 * @param id ID of the object.
	 * @param x starting position x coordinate.
	 * @param y starting position y coordinate.
	 * @param mvType pointer to MV_TRJ class instance which handles this object movement.
	 * @param spd moving speed.
	 * @param bnd not used yet.
	 */
	MovingObject(	int id, int x = 0, int y = 0, 
					PMV_TRJ mvType = 0,  float spd = 0,
					BND_REL bnd = SBRX_BND_IGNORE );
	/**
	 * Destructor.
	 */
	virtual		~MovingObject();

	/**
	 * Sets specified move type as new motion trajectory.
	 * @param mvType 
	 */
				void		SetMoveType( PMV_TRJ mvType );

	__declspec( property( get=GetSpeed, put=SetSpeed ) ) FLOAT Speed;
	/**
	 * Property put method for _fSpeed.
	 * @param fSpeed new value of motion speed.
	 */
				void		SetSpeed( FLOAT fSpeed );
	/**
	 * Property get method for _fSpeed.
	 * @return value of motion speed.
	 */
				float		GetSpeed() const { return _fSpeed; }
	/**
	 * Orders object to move to new position with along its trajectory.
	 * @param mvx coordinate of destination point along x axis.
	 * @param mvy coordinate of destination point along y axis.
	 */
				void		MoveTo( int mvx, int mvy );
	/**
	 * Stops object from moving.
	 */
				void		Stop() { _bMoving = false; } 
	/**
	 * Orders object to continue its moving.
	 */
				void		Continue();
	/**
	 * Returns object' moving state.
	 * @return true if object are currently moving, otherwise false.
	 */
				bool		IsMoving() const { return _bMoving; }
	/**
	 * Allows object to process its state.
	 * Trajectory Step method invokes here.
	 * @param fTimeDelta time in msc's since last call to this method.
	 */
	virtual		void		Process( FLOAT fTimeDelta );
};

//-------------------------------------------------------------

/**
 * Concept of static object - an object which cann't move and therefore it doesn't invokes any collisions.
 * All concrete objects should be inherited from this class.
 */
class StaticObject: public Object
{
public:
	/**
	 * Constructor.
	 * @param id ID of the object.
	 * @param x starting position x coordinate.
	 * @param y starting position y coordinate.
	 */
	StaticObject( int id, int x = 0, int y = 0 );
	/**
	 * Destructor.
	 */
	virtual	~StaticObject() { }
};

//-------------------------------------------------------------

/**
 * Concept of the effect: object which looks like some kind of static and moving objects mixed together.
 * In can invokes collisions named affects and has short "lifetime" - it appears, do something stupid and after that "disappears" (but it could stay it memory).
 * Examples of effect objects are explosives, energy field, blastwaves, etc.
 */
class Effect: public Object
{
protected:
	SbrAnim*		_pAnim;					/**< Pointer to animation which represents this effect on the screen. */

public:
	/**
	 * Constructor.
	 * @param id ID of the object.
	 * @param anim pointer to animation class instance.
	 */
	Effect( int id, SbrAnim* anim );
	/**
	 * Destructor.
	 */
	virtual	~Effect();

	/**
	 * Allow effect to take it effect (some kind of joke, huh?) and shows it on the screen.
	 * @param x position coordinate along x axis effect will be shown at.
	 * @param y position coordinate along x axis effect will be shown at.
	 */ 
				void	Perform( int x, int y );
	/**
	 * Stops effect' animation and remove it from the screen.
	 */
				void	Stop();
	/**
	 * Handles collision with other objects.
	 * Inheritance from this class and redefining this method allow you to create your own effects which can do some additional work.
	 * @param obj pointer to object class instance this object is collided with.
	 */
	virtual		void	OnCollide( Object* obj );
	/**
	 * Allows effect to process its state.
	 * @param fTimeDelta time in msc's since last call to this method.
	 */
	virtual		void	Process( FLOAT fTimeDelta );
	/**
	 * Renders animation associated with this object on the screen.
	 */
	virtual		void	Render();
	/**
	 * Sets new animation for this effect.
	 * @param spr pointer to new sprite for this object.
	 */
				void	SetAnim( SbrAnim* anim );
};

//-------------------------------------------------------------

/**
 * Main Saber Phys Engine concept - the World.
 * World respesents two-dimessional field on which object can move along their trajecories and interract with each other.
 * World have its own bounaries, so it is possible to deremine when object leave its world it belongs to.
 * Any object after creation autamically registering in the World, so if client losts pointer to this object, World is still remember it.
 * TODO: Corrent boudaries check, multiworld structure.
 */
class World {
private:
	static	World*	_self;							/**< pointer to only instance of this class. */
	static	int		_nRefCount;						/**< number of references created to only instance of this class. */

	/**
	 *
	 */
	World( int w, int h, int x = 0, int y = 0 );
	/**
	 * Destructor.
	 * Destroys all objects registered in the World.
	 */
	~World();

private:
	std::vector<Object*>	_vObjects;			/**< vector of objects registered in the World. */
	RECT					_rcBorders;			/**< RECT struct which holds World's bounaries. */

public:
	/**
	 * Method used for receiving pointer to instance of the World.
	 * @param w width of newly created World (only valid if there are none).
	 * @param h height of newly created World (only valid if there are none).
	 * @param x offset of upper-left corner of the World from upper-left corner of game window alongside x axis.
	 * @param y offset of upper-left corner of the World from upper-left corner of game window alongside y axis.
	 * @return pointer to World instance.
	 */
	static	World*	Instance( int w = -1, int h = -1, int x = -1, int y = -1 );
	/**
	 *	Releases reference to the World.
	 */
			void	Release();
	
	/**
	 * Registers object in the World.
	 * @param obj pointer to the Object to register.
	 */
			void	AddObj( Object* obj );
	/** 
	 * Unregisters object in the World if you on your own purpose wants to do so.
	 * @param obj pointer to the Object you want to unregister.
	 */
			void	DeleteObj( Object* obj );
	/**
	 * Allows all objects registered in the World to perform their Process method and checks for collision.
	 * If there are one of them happens invokes corresponding OnCollide methods.
	 * @param fTimeDelta time elapsed since last call to this method.
	 */
			void	ProcessAll( float fTimeDelta );
	/**
	 * Renders all objects registered in the World.
	 * Usually call to this method is needed after call to ProcessAll to show user changes in the World since last frame.
	 */
			void	RenderAll();
	/**
	 * Returns World's boundaries.
	 * @return rect pointer to RECT struct wich receives boundary's dimessions.
	 */
			void	GetBorders( RECT* rect ) const;
};

#endif

