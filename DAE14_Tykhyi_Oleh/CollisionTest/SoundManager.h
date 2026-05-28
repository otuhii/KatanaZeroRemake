#pragma once
#include <vector>

class SoundEffect;
class SoundStream;
class LevelManager;

class SoundManager final
{
public:
	enum class SoundEffectType {
		catPet,
		meow,
		playerJump,
		bulletDie,
		enemyPunch,
		enemyHit,
		gunFire,
		doorKick,
		footstep,
		land,
		masterswordCharged,
		masterswordImpact,
		masterswordSlash,
		playerPrerun,
		roll,
		bulletSlash,
		masterswordBulletDie,
		plantBreak,
		playerDie,
		enemyDeathBullet,
		enemyDeathMasterSword,
		enemyDeathRegular,
		enemyDeathSword,

		count
	};

	enum class SoundStreamTypes {
		none=-1,
		
		youWillNeverKnow,

		count
	};

	SoundManager();

	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	void LinkLevelManager(LevelManager* pLevelManager);

	~SoundManager();

	void Play(SoundEffectType type, int replayCount) const;
	void Stop(SoundEffectType type) const;
private:
	LevelManager*
		m_pLevelManager{};


	std::vector<SoundEffect*> m_pSoundEffects{};
	std::vector<SoundStream*> m_pSoundStreams{};
};

