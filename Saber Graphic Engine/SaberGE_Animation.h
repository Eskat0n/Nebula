//-------------------------------------------------------------
// Saber Graphic Engine animation class declaration file
// Written by Eskat0n
// Current engine version: 0.0.8
// Modified: 6 nov 2008
//-------------------------------------------------------------

#include "SaberGE_Sprite.h"

#ifndef _SBR_ANIM_CLASS_
#define _SBR_ANIM_CLASS_

//-------------------------------------------------------------
// Definitions of animation behaviour flags
//-------------------------------------------------------------

#define SBRAP_FORWARD	0
#define	SBRAP_LOOP		1
#define SBRAP_NOLOOP	0
#define SBRAP_DNTINTER  2

//-------------------------------------------------------------

/**
 * Animation class.
 * Indeed, animation is a texture, which combines many animation frames into one, or mor rows.
 * also animation can be used to render objects with many action phases such as rotation tank towers etc. 
 * So it works alike animation on my favourite SNES console. 
 * So I simply derive this animation class concept from Nindendo framechains.
 * I hope their'll forgive me for this.
 */
class SbrAnim: public SbrSprite
{
protected:
	/**
	 * Default constructor.
	 */
	SbrAnim();

	INT			_orig_width;				/**< Overall width of original texture. */

	BOOL		_bIsPlaying;				/**< True if animation are currently playing, otherwise false. */

	FLOAT		_fSpeed;					/**< Animation playback speed. */
	FLOAT		_fTimeDeltaPF;				/**< Time elapsed since animation frame are rendered. */

	INT			_mode;						/**< Animation playback mode. */
	INT			_nDelta;					/**< Framestep. */
	INT			_nFrames;					/**< Total number of frames of animation. */
	INT			_nCurFrame;					/**< Current animation frame. */

public:
	/**
	 * Creates animation with specified parameters.
	 * @param textname null-terminated unicode string which contains path to the texture of new sprite.
	 * @param nFrames total number of frames of created animation.
	 * @param FPS frames-per-second ratio which determines animation speed.
	 * @param width animation zone width in pixels (same as sprite width).
	 * @param height animation zone height in pixels (same as sprite height).
	 * @param x displacement of texture along X axis (upper-left corner relative).
	 * @param y displacement of texture along Y axis (upper-left corner relative).
	 */
	SbrAnim( LPCWSTR texname, int nFrames, float FPS, float width, float height, float x = 0, float y = 0 );

	/**
	 * Starts animation playback.
	 */
	void		Play();
	/**
	 * Stops animation playback.
	 */
	void		Stop() { _bIsPlaying = false; }
	/**
	 * Resume animation playback from stopped position.
	 */
	void		Resume() { _bIsPlaying = true; }
	/**
	 * Allow animation to update its state and undate current frame if needed.
	 * @param fTimeDelta time elapsed since last call to this method.
	 */
	void		Update( float fTimeDelta );
	/**
	 * Returns current animation state.
	 * @return true if animation are playing now, otherwise false.
	 */ 
	bool		IsPlaying() const { return _bIsPlaying; }
	/** 
	 * Sets new texture for this animation.
	 * @param tex id of the texture.
	 */
	void		SetTexture( SBRTEXTURE tex ) {	SbrSprite::SetTexture( tex ); 
												_orig_width = _sbr->GetTextureWidth( tex ); }
	/** 
	 * Sets animation playback mode.
	 * @param mode animation playback mode.
	 */
	void		SetMode( int mode );
	/**
	 * Sets animation playback speed in frames per second (FPS).
	 * @param FPS value of FPS.
	 */
	void		SetSpeed( float FPS ) { _fSpeed = ( FPS != 0 ) ? (1.0f / FPS):1.0f; }
	/**
	 * Sets current frame of animation.
	 * @param number of the frame to be setted.
	 */
	void		SetFrame( int n );
	/**
	 * Sets overall number of frames for this animation.
	 * @param number of frames.
	 */
	void		SetFrames( int n ) { _nFrames = n; }
	/**
	 * Returns current animation mode.
	 * @param animation mode.
	 */
	int			GetMode() const { return _mode; }
	/**
	 * Returns value of animation speed (in secs between two nearest animation frames).
	 * @return animation speed.
	 */
	float		GetSpeed() const { return _fSpeed; }
	/**
	 * Returns current frame of animation.
	 * @return number of animation frame.
	 */
	int			GetFrame() const { return _nCurFrame; }
	/**
	 * Return overall number of this animation frames.
	 * @return number of animation frames.
	 */
	int			GetFrames() const { return _nFrames; }
};

#endif