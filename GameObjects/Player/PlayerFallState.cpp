#include "PlayerFallState.h"
#include "Player.h"


PlayerFallState::PlayerFallState()
{
	//don't use
}

PlayerFallState::PlayerFallState(Player * player) : PlayerState(player, PlayerState::StateName::Fall)
{
	SetAnimation(new Animation(ResourceManager::GetInstance()->GetAnimationXMLAladdin(), "Fall", ResourceManager::GetInstance()->GetTextureAladdin(), 0.8f, false));

	_player->SetAccelerationY(_player->GetMass());
}


PlayerFallState::~PlayerFallState()
{
}

void PlayerFallState::Update(float deltaTime)
{
	PlayerState::Update(deltaTime);

	if (_player->IsGround())//change state
	{
		_player->SetState(new PlayerIdleState(_player));
		return;
	}

	if (Input::GetInstance()->IsKeyPressed(DIK_S))
	{
		_player->SetState(new PlayerJumpAttackState(_player));
		return;
	}

	if (Input::GetInstance()->IsKeyPressed(DIK_A))
	{
		_player->SetState(new PlayerJumpThrowState(_player));
		return;
	}
}

void PlayerFallState::OnCollision(GameObject * target, GameCollision::SideCollisions side)
{
	PlayerState::OnCollision(target, side);

	if (target->GetTag() == GameObject::GameObjectType::Rope)
	{
		float diffX = abs(target->GetPosition().x - _player->GetPosition().x);
		if (diffX < 18 && _player->GetBound().bottom <= target->GetBound().bottom && _player->GetBound().top >= target->GetBound().top)
		{
			_player->SetState(new PlayerClimbVerticalState(_player, target));
		}
	}

	if (target->GetTag() == GameObject::GameObjectType::HorizontalBar)
	{
		float diffTop = abs(target->GetBound().top - _player->GetBound().top);
		if (diffTop < 18
			&& _player->GetPosition().x >= target->GetBound().left
			&& _player->GetPosition().x <= target->GetBound().right)
			_player->SetState(new PlayerClimbHorizontalIdleState(_player, target), false);
	}

	if (target->GetTag() == GameObject::GameObjectType::Springboard)
	{
		float diffY = abs(target->GetPosition().y - _player->GetBound().bottom);//ko dung GetBound, ma lay positionY cua target vi target nam xieng
		if (diffY < 18)
		{
			_player->SetState(new PlayerSpringState(_player));
		}
	}
}
