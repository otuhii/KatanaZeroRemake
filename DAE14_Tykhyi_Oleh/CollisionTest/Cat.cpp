#include "pch.h"
#include "Cat.h"

#include "SoundManager.h"
#include "Player.h"

Cat::Cat(Sprite* pSprite, const std::vector<AnimationFrameInfo>& catAnimation, const Vector2f& position, int floor, float interactionRange, float scale)
	: InteractableObject{ position, floor, interactionRange, InteractableType::cat },
	m_pSprite{ pSprite },
	m_CatSpriteFrames{ catAnimation }
{
	m_pSprite->SetScale(scale);
	SetState(CatState::idle, nullptr);
}

void Cat::Draw() const
{
	m_pSprite->Draw(GetPosition(), true, false);
}

void Cat::Update(float elapsedSec, SoundManager* pSoundManager, Player* pPlayer)
{
	switch (m_State)
	{
	case CatState::idle:
	{
		int
			minIdleDuration{ 4 },
			maxIdleDuration{ 8 },
			lickChance{ 6 };

		m_IdleTimer += elapsedSec;
		if (m_IdleTimer >= m_IdleDuration)
		{
			m_IdleTimer = 0.f;
			m_IdleDuration = static_cast<float>((rand() % (maxIdleDuration - minIdleDuration + 1)) + minIdleDuration);


			if (rand() % 10 < 3)
			{
				SetState(CatState::toLick, pSoundManager);
			}
			else
			{
				if (IsPlayerInRange(pPlayer->GetPosition(), pPlayer->GetFloor()))
				{
					SetState(CatState::lookBack, pSoundManager);
				}
				else
				{
					SetState(CatState::toLick, pSoundManager);
				}
			}
		}
		break;
	}
	case CatState::toLick:
	{
		if (m_pSprite->IsFinished())
		{
			int
				minLickCycles{ 2 },
				maxLickCycles{ 4 };


			m_LickCycles = 0;
			m_TargetLickCycles = (rand() % (maxLickCycles - minLickCycles + 1)) + minLickCycles;

			SetState(CatState::licking, pSoundManager);
		}
		break;
	}
	case CatState::licking:
	{
		if (m_pSprite->IsFinished())
		{
			m_LickCycles++;
			if (m_LickCycles >= m_TargetLickCycles)
			{
				SetState(CatState::fromLick, pSoundManager);
			}
		}

		break;
	}
	case CatState::fromLick:
	{
		if (m_pSprite->IsFinished())
		{
			SetState(CatState::idle, pSoundManager);
		}
		break;
	}
	case CatState::lookBack:
	{
		if (m_pSprite->IsFinished())
		{
			SetState(CatState::idle, pSoundManager);
		}
		break;
	}
	case CatState::petting:
	{
		if (m_pSprite->IsFinished())
		{
			SetState(CatState::idle, pSoundManager);
		}
		break;
	}
	}

}

void Cat::SetState(CatState state, SoundManager* pSoundManager)
{
	if (m_State == state) { return; }

	m_State = state;

	if (m_State == CatState::petting)
	{
		pSoundManager->Play(SoundManager::SoundEffectType::catPet, 0);
	}
	else if (m_State == CatState::lookBack)
	{
		pSoundManager->Play(SoundManager::SoundEffectType::meow, 0);
	}

	m_pSprite->SetAnimationFrameInfo(m_CatSpriteFrames[static_cast<int>(m_State)]);
	m_pSprite->ResetAnimation();
}

void Cat::Interact(SoundManager* pSoundManager)
{
	SetState(CatState::petting, pSoundManager);
}
