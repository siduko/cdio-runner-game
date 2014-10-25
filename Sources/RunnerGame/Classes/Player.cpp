#include "Player.h"


Player::Player(ValueMap properties) : GameObject(properties)
{
	_entityManager->addComponentObject("Animator", Animator::create());
	Animator* animator = (Animator*)this->getEntityManager()->getComponentObjectByName("Animator");
	animator->addAction("idle", properties["ImageCount"].asInt(), properties["ImagePath"].asString());
	animator->addAction("run", properties["RunCount"].asInt(), properties["RunPath"].asString());
	animator->addAction("hurt", properties["HurtCount"].asInt(), properties["HurtPath"].asString());
	animator->addAction("jump", properties["JumpCount"].asInt(), properties["JumpPath"].asString());
	_playerState = PlayerState::Idle;
	scheduleUpdate();
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

void Player::move(Vec2 vec)
{

}

void Player::jump(Vec2 vec)
{
	if (_allowJump){
		this->getPhysicsBody()->applyImpulse(vec);
		_allowJump = false;
	}
}

void Player::setSpeed(float value)
{
	//playerRun->setSpeed(value);
}

void Player::update(float dt)
{
	if (this->getPhysicsBody()->getVelocity().x < 0)
		_playerState = PlayerState::Hurt;
	else if (this->getPhysicsBody()->getVelocity().x >= 0 && this->getPhysicsBody()->getVelocity().x < 1)
	{
		if (_playerState == PlayerState::Running)
		{
			this->getPhysicsBody()->applyImpulse(ccp(-100000, 1000000));
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
	if (this->getPhysicsBody()->getVelocity().x < 100)
		this->getPhysicsBody()->setVelocity(ccpAdd(this->getPhysicsBody()->getVelocity(), ccp(10*dt, 0)));
}
