#include "pch.h"
#include "SoundManager.h"

#include "SoundEffect.h"

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

	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyPunch)] = new SoundEffect{ "sounds/sound_enemy_punch.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::enemyHit)] = new SoundEffect{ "sounds/sound_enemy_punch_hit.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::gunFire)] = new SoundEffect{ "sounds/sound_gun_fire.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::doorKick)] = new SoundEffect{ "sounds/sound_player_doorkick.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::footstep)] = new SoundEffect{ "sounds/sound_player_footstep.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::land)] = new SoundEffect{ "sounds/sound_player_land.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordCharged)] = new SoundEffect{ "sounds/sound_player_mastersword_charged_01.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordImpact)] = new SoundEffect{ "sounds/sound_player_mastersword_impact_01.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::playerPrerun)] = new SoundEffect{ "sounds/sound_player_prerun.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::roll)] = new SoundEffect{ "sounds/sound_player_roll.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::bulletSlash)] = new SoundEffect{ "sounds/sound_slash_bullet.wav" };

	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordBulletDie)] = new SoundEffect{ "sounds/sound_mastersword_particle_die.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::masterswordBulletDie)]->SetVolume(5);

	m_pSoundEffects[static_cast<int>(SoundEffectType::plantBreak)] = new SoundEffect{ "sounds/plant_break.wav" };
	m_pSoundEffects[static_cast<int>(SoundEffectType::plantBreak)]->SetVolume(10);
}

SoundManager::~SoundManager()
{
	for (SoundEffect* pSoundEffect : m_pSoundEffects)
	{
		delete pSoundEffect;
	}
}

void SoundManager::Play(SoundEffectType type, int replayCount) const
{
	m_pSoundEffects[static_cast<int>(type)]->Play(replayCount);
}

void SoundManager::Stop(SoundEffectType type) const
{
	m_pSoundEffects[static_cast<int>(type)]->StopAll();
}
