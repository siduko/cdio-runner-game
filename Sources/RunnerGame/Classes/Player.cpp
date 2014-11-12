#include "Player.h"


Player::Player(ValueMap properties) : GameObject(properties)
{
	_entityManager->addComponentObject("Animator", Animator::create());
	Animator* animator = (Animator*)this->getEntityManager()->getComponentObjectByName("Animator");
	animator->addAction("idle", properties["ImageCount"].asInt(), properties["ImagePath"].asString());
	animator->addAction("run", properties["RunCount"].asInt(), properties["RunPath"].asString());
	animator->addAction("hurt", properties["HurtCount"].asInt(), properties["HurtPath"].asString());
	animator->addAction("jump", properties["JumpCount"].asInt(), properties["JumpPath"].asString());
	_entityManager->addComponentObject("EffectComponent", EffectComponent::create());
	_playerState = PlayerState::Idle;
	_acceleration = 20;
	_limitVelocity = 500;
	_score = 0;
}


Player::~Player()
{
}

Player* Player::create(ValueMap properties)
{
	Player *sprite = new Player(properties);
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Player::jump()
{
	if (_allowJump){
		this->getPhysicsBody()->applyImpulse(ccp(0, 700000));
		_allowJump = false;
	}
}

void Player::update(float dt)
{
	GameObject::update(dt);
	if (this->getVelocity() < 0)
		_playerState = PlayerState::Hurt;
	else if (this->getVelocity() >= 0 && this->getVelocity() < 1)
	{
		if (_playerState == PlayerState::Running)
		{
			this->getPhysicsBody()->applyImpulse(ccp(-100000, 10000));
		}
		else
			_playerState = PlayerState::Idle;
	}
	else
	{
		_playerState = PlayerState::Running;
	}
	switch (_playerState)
	{
	case Idle:
		((Animator*)this->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("idle", 0.2f, true);
		break;
	case Running:
		((Animator*)this->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("run", 1.0f, true);
		break;
	case Jumping:
		((Animator*)this->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("jump", 1.2f, false, true);
		break;
	case Hurt:
		((Animator*)this->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("hurt", 2.0f);
		break;
	default:
		break;
	}
	EffectComponent* effectPlayer = ((EffectComponent*)this->getEntityManager()->getComponentObjectByName("EffectComponent"));
	if (effectPlayer->getAlive())
	{
		if (!(effectPlayer->getRunningEffect() == EffectType::FastEffect || effectPlayer->getRunningEffect() == EffectType::SlowEffect))
		{
			if (this->getVelocity() < _limitVelocity)
				this->setVelocity(this->getVelocity() + _acceleration * dt);
		}
	}
	else
	{
		if (this->getVelocity() < _limitVelocity)
			this->setVelocity(this->getVelocity() + _acceleration * dt);
	}

}
