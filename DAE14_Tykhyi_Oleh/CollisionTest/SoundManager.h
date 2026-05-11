#pragma once

class SoundEffect;
class SoundStream;

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
		playerPrerun,
		roll,
		bulletSlash,
		masterswordBulletDie,
		plantBreak,

		count
	};

	SoundManager();

	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	~SoundManager();

	void Play(SoundEffectType type, int replayCount) const;
	void Stop(SoundEffectType type) const;
private:
	std::vector<SoundEffect*> m_pSoundEffects{};
	std::vector<SoundStream*> m_pSoundStream{};
};

