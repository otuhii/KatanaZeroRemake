#include "pch.h"
#include "SoundManager.h"

#include "SoundEffect.h"
#include "SoundStream.h"

#include "LevelManager.h"

SoundManager::SoundManager()
{
	m_pSoundEffects.resize(static_cast<int>(SoundEffectType::count));

	m_pSoundEffects[static_cast<int>(SoundEffectType::catPet)] = new SoundEffect{ "sounds/catPet.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::catPet)]->SetVolume(15);

	m_pSoundEffects[static_cast<int>(SoundEffectType::meow)] = new SoundEffect{ "sounds/meeeeeow.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::meow)]->SetVolume(15);

	m_pSoundEffects[static_cast<int>(SoundEffectType::playerJump)] = new SoundEffect{ "sounds/player_jump.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::playerJump)]->SetVolume(15);

	m_pSoundEffects[static_cast<int>(SoundEffectType::bulletDie)] = new SoundEffect{ "sounds/sound_bulletdie.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::bulletDie)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyPunch)] = new SoundEffect{ "sounds/sound_enemy_punch.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyPunch)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyHit)] = new SoundEffect{ "sounds/sound_enemy_punch_hit.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyHit)]->SetVolume(5);

	m_pSoundEffects[static_cast<int>(SoundEffectType::gunFire)] = new SoundEffect{ "sounds/sound_gun_fire.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::gunFire)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::doorKick)] = new SoundEffect{ "sounds/sound_player_doorkick.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::doorKick)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::footstep)] =  new SoundEffect{ "sounds/sound_player_footstep.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::footstep)]->SetVolume(5);

	m_pSoundEffects[static_cast<int>(SoundEffectType::land)] = new SoundEffect{ "sounds/sound_player_land.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::land)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordCharged)] = new SoundEffect{ "sounds/sound_player_mastersword_charged_01.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordImpact)] = new SoundEffect{ "sounds/sound_player_mastersword_impact_01.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::playerPrerun)] = new SoundEffect{ "sounds/sound_player_prerun.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::playerPrerun)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::roll)] = new SoundEffect{ "sounds/sound_player_roll.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::roll)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::bulletSlash)] = new SoundEffect{ "sounds/sound_slash_bullet.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::bulletSlash)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordBulletDie)] = new SoundEffect{ "sounds/sound_mastersword_particle_die.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordBulletDie)]->SetVolume(5);

	m_pSoundEffects[static_cast<int>(SoundEffectType::plantBreak)] = new SoundEffect{ "sounds/plant_break.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::plantBreak)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordSlash)] = new SoundEffect{ "sounds/sound_player_mastersword_slash.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordSlash)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::playerDie)] = new SoundEffect{ "sounds/player_die.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::playerDie)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyDeathBullet)] = new SoundEffect{ "sounds/enemy_death_bullet.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyDeathBullet)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyDeathRegular)] = new SoundEffect{ "sounds/enemy_death_regular.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyDeathRegular)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyDeathSword)] = new SoundEffect{ "sounds/enemy_death_sword.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyDeathSword)]->SetVolume(10);

	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyDeathMasterSword)] = new SoundEffect{ "sounds/enemy_death_mastersword.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyDeathMasterSword)]->SetVolume(10);

	m_pSoundStreams.resize(static_cast<int>(SoundStreamTypes::count));

	m_pSoundStreams[static_cast<int>(SoundStreamTypes::youWillNeverKnow)] = new SoundStream{ "sounds/song_youwillneverknow.ogg" };
	m_pSoundStreams[static_cast<int>(SoundStreamTypes::youWillNeverKnow)]->SetVolume(25);
	m_pSoundStreams[static_cast<int>(SoundStreamTypes::youWillNeverKnow)]->Play(true);
}

SoundManager::~SoundManager()
{
	for (SoundEffect* pSoundEffect : m_pSoundEffects)
	{
		delete pSoundEffect;
	}

	for (SoundStream* pSoundStream : m_pSoundStreams)
	{
		delete pSoundStream;
	}
}

void SoundManager::LinkLevelManager(LevelManager* pLevelManager)
{
	m_pLevelManager = pLevelManager;
}

void SoundManager::Play(SoundEffectType type, int replayCount) const
{
	m_pSoundEffects[static_cast<int>(type)]->Play(replayCount);

	if (m_pLevelManager)
	{
		m_pLevelManager->RecordSound(type);
	}
}

void SoundManager::Stop(SoundEffectType type) const
{
	m_pSoundEffects[static_cast<int>(type)]->StopAll();
}
