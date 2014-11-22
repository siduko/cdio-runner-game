#include "Player.h"


Player::Player(ValueMap properties) : GameObject(properties)
{
	_entityManager->addComponentObject("Animator", Animator::create());
	Animator* animator = (Animator*)this->getEntityManager()->getComponentObjectByName("Animator");
	animator->addAction("idle", properties["ImageCount"].asInt(), properties["ImagePath"].asString());
	animator->addAction("run", properties["RunCount"].asInt(), properties["RunPath"].asString());
	animator->addAction("hurt", properties["HurtCount"].asInt(), properties["HurtPath"].asString());
	animator->addAction("jump", properties["JumpCount"].asInt(), properties["JumpPath"].asString());
	animator->addAction("duck", properties["DuckCount"].asInt(), properties["DuckPath"].asString());
	animator->addAction("climb", properties["ClimbCount"].asInt(), properties["ClimbPath"].asString());
	animator->addAction("swim", properties["SwimCount"].asInt(), properties["SwimPath"].asString());
	_entityManager->addComponentObject("EffectComponent", EffectComponent::create());
	_playerState = PlayerState::Running;
	_acceleration = 20;
	_limitVelocity = DataController::getInstance()->getGameSettings()["PlayerVelocityLimit"].asInt();
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

void Player::jump(Vec2 vec)
{
	if (_allowJump){
		this->getPhysicsBody()->applyImpulse(vec);
		_allowJump = false;
	}
}

void Player::update(float dt)
{
	GameObject::update(dt);
	Animator* ani = (Animator*)this->getEntityManager()->getComponentObjectByName("Animator");
	switch (_playerState)
	{
	case Idle:
		ani->playActionByName("idle", 0.2f, true);
		break;
	case Running:
		ani->playActionByName("run", 1.0f, true);
		if (_autoControlVelocity){
			if (this->getVelocity() < _limitVelocity)
				this->setVelocity(this->getVelocity() + _acceleration * dt);
			else
				this->setVelocity(_limitVelocity);
			ani->getPlayingAction()->setSpeed(this->getVelocity() / 30);
		}
		break;
	case Jumping:
		ani->playActionByName("jump", 1.2f);
		break;
	case Hurt:
		ani->playActionByName("hurt", 2.0f);
		_actionTimeOut -= dt;
		if (_actionTimeOut < 0)
			_playerState = PlayerState::Running;
		break;
	case Swim:
		((Animator*)this->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("swim", 1.0f,true);
		break;
	case Duck:
		((Animator*)this->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("duck", 2.0f);
		break;
	case Climb:
		((Animator*)this->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("climb", 1.0f,true);
		break;
	case Falling:
		((Animator*)this->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("jump", 1.0f, true);
		break;
	}
}
