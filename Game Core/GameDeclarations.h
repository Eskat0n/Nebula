//-------------------------------------------------------------
// Game constants
//-------------------------------------------------------------

#ifndef _GAME_DECLARATIONS_
#define _GAME_DECLARATIONS_

// Objects IDs

#define	DUMMY				0

#define PLSHIP01			1
#define PLSHIP02			2
#define BONUS01				3
#define BONUS02				4
#define BONUS03				5
#define ENEMY01				6
#define ENEMY02				7
#define ENEMY03				8
#define ENEMY04				9
#define ENEMY_BOSS			10
#define SHOT_PLAYER			11
#define SHOT_PLAYER_ROCKET	12
#define SHOT_ENEMY			13
#define SHOT_ENEMY_FST		14
#define SHOT04				15
#define EXPLOSIVE			16

// Armor parameters

#define PLSHIP_ARMOR		1
#define POD_ARMOR			1
#define BORG_ARMOR			1
#define TRES_ARMOR			2
#define GUNGIR_ARMOR		3
#define BOSS_ARMOR			25

// Damage paremeters

#define BLASTER_DMG			1
#define ROCKET_DMG			5

// Ships speed (px/sec)

#define PLSHIP_SPEED
#define ENEMY_SPEED
#define	BORG_SPEED

// Weapons reload time (in secs)

#define BLASTER_RELOAD_TIME 0.45f
#define ROCKETS_RELOAD_TIME 2.0f

#define POD_RELOAD_TIME		2.0f
#define BORG_RELOAD_TIME	1.5f
#define TRES_RELOAD_TIME	3.5f
#define GUNGIR_RELOAD_TIME	2.0f
#define BOSS_RELOAD_TIME	1.5f

// Abilities reload time (in secs)

#define POD_ARELOAD_TIME	600.0f
#define BORG_ARELOAD_TIME	6.0f
#define TRES_ARELOAD_TIME	8.0f
#define GUNGIR_ARELOAD_TIME	5.0f
#define BOSS_ARELOAD_TIME	3.0f
#define BOSS_ACOOLDOWN		7.0f

#define MB_FULL_FADE		1.3f

// Weapons shot speed (px/sec)

#define BLASTER_SPEED		450
#define ROCKET_SPEED		600
#define POD_SHOT_SPEED		250
#define BORG_SHOT_SPEED		300
#define TRES_SHOT_SPEED		250
#define	TRES_SFST_SPEED		350
#define GUNGIR_SHOT_SPEED	500
#define BOSS_SHOT_SPEED		900

// Enemy shooting chances (in percents)

#define POD_SHOOT_CHANCE	0.3f
#define BORG_SHOOT_CHANCE	1.0f
#define TRES_SHOOT_CHANCE	1.75f
#define GUNGIR_SHOOT_CHANCE	3.33f
#define BOSS_SHOOT_CHANCE	100.0f

// Enemy ability chances (in percents)

#define POD_ABL_CHANCE		0.0005f
#define BORG_ABL_CHANCE		0.05f
#define TRES_ABL_CHANCE		0.5f
#define GUNGIR_ABL_CHANCE	0.15f
#define BOSS_ABL_CHANCE		7.0f

// Bonuses dropping chances (in percents)

#define POD_BONUS_CHANCE	0.60f
#define BORG_BONUS_CHANCE	0.75f
#define TRES_BONUS_CHANCE	1.0f
#define GUNGIR_BONUS_CHANCE	5.0f

// Cost of ships (in game points)

#define POD_COST			100
#define BORG_COST			200
#define TRES_COST			500	
#define GUNGIR_COST			750
#define BOSS_COST			5000;

// Texture files

#define PLSHIP_TEX			L"./gfx/sprites/plship.bmp"
#define POD_TEX				L"./gfx/sprites/pod.bmp"
#define BORG_TEX			L"./gfx/sprites/borg.bmp"
#define TRES_TEX			L"./gfx/sprites/tres.bmp"
#define GUNGIR_TEX			L"./gfx/sprites/gungir.bmp"
#define BOSS_TEX			L"./gfx/sprites/final_boss.bmp"
#define PLSHOT_TEX			L"./gfx/sprites/shot00.bmp"
#define ROCKET_TEX			L"./gfx/sprites/rocket.bmp"
#define ENEMYSHOT_TEX		L"./gfx/sprites/shot01.bmp"
#define BOSS_SHOT_TEX		L"./gfx/sprites/boss_shot.bmp"
#define EXPLOSIVE_TEX		L"./gfx/sprites/explosive.bmp"
#define MEGABLAST_TEX		L"./gfx/sprites/mega_blast.bmp"
#define LIFE_TEX			L"./gfx/sprites/life.bmp"

// Background image files

#define BKGR01				L"./gfx/backgrounds/bkgr_lvl01-02.png"
#define BKGR02				L"./gfx/backgrounds/bkgr_lvl01-02.png"
#define BKGR03				L"./gfx/backgrounds/bkgr_lvl03-04.png"
#define BKGR04				L"./gfx/backgrounds/bkgr_lvl03-04.png"
#define BKGR05				L"./gfx/backgrounds/bkgr_lvl05.png"
#define BKGR06				L"./gfx/backgrounds/bkgr_lvl06-07.png"
#define BKGR07				L"./gfx/backgrounds/bkgr_lvl06-07.png"
#define BKGR08				L"./gfx/backgrounds/bkgr_lvl08.png"
#define BKGR09				L"./gfx/backgrounds/bkgr_lvl09.png"
#define BKGR10				L"./gfx/backgrounds/bkgr_lvl10.png"

// Bank files

#define EFFECT_BANK			L"./sound/EffectBank.xwb"
#define MUSIC_BANK			L"./sound/MusicBank.xwb"
#define SOUND_BANK			L"./sound/SoundBank.xsb"

// Sound files

#define PLSHOT_MX			"blaster"
#define ROCKET_MX			"rocket"
#define EXPLOSION_MX		"explosion"
#define	BOSS_SHOT_MX		"boss_blaster"
#define MEGABLAST_MX		"boss_megablaster"

// Music files

#define STARTSCREEN_MX		"freedom"
#define INGAME_MX			"spacetheme"

// Text constants

#define NEBULA_WINDOW		L"Nebula beta02"

#define MISSION01			L" Mission 1: When they comes  "
#define MISSION02			L"  Mission 2: Hostile skies   "
#define MISSION03			L"    Mission 3: No pasaran!   "
#define MISSION04			L" Mission 4: Farewell, Earth! "
#define MISSION05			L" Mission 5: Jupiter thunder  "
#define MISSION06			L" Mission 6: Crying asteroids "
#define MISSION07			L"Mission 7: In rings of Saturn"
#define MISSION08			L"Mission 8: Never say \"never\" "
#define MISSION09			L"   Mission 9: Deep in space  "
#define MISSION10			L"     Mission 10: Nebula      "

#define COMPLETE			L"      Mission complete!      "
#define GAMEOVER			L"          Game Over          "
#define GAMECOMPLETE		L"               Congratulations!\nYou are succesfully save the Earth\n      but could you return home?\n   The space story isn't over yet!"

#define CREDITS				L"Staff:\nGame engine: Eskat0n\nNetplay, interface: myst\nSpecial thanks to Pavel Tsytovich\n\nOpening song: HEAVENS WiRE - STAY for FREEDOM\n\nMuyouDev, 2008\n\nOur next game will be Nebula: Heaven Drive. Coming this winter!"

#define COMBOLVL01			L"Double kill"
#define	COMBOLVL02			L"Triple kill"
#define COMBOLVL03			L"Killing spree"
#define COMBOLVL04			L"Massacre!"
#define COMBOLVL05			L"Demon of space!"

// Time parameters

#define MISSION_INTERVAL	3.0f
#define COMPLETE_INTERVAL	3.0f
#define GAME_COMPLETE		5.0f
#define CREDITS_TIME		60.0f
#define GAME_OVER			4.0f
#define PL_SPAWN_TIME		3.0f
#define PL_IMMORTAL_TIME	1.5f

#endif