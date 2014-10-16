#include "Player.h"


Player::Player()
{
	//this->playerRun = NULL;
}


Player::~Player()
{
}

Player* Player::create()
{
	Player *sprite = new Player();
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
	spriteBody->applyImpulse(vec);
}

void Player::jump(Vec2 vec)
{
	if (_allowJump){
		spriteBody->applyImpulse(vec);
		_allowJump = false;
	}
}

void Player::setInfo(ValueMap properties)
{
	GameObject::setInfo(properties);
	_animator->addAction("idle", properties["ImageCount"].asInt(), properties["ImagePath"].asString());
	_animator->addAction("run", properties["RunCount"].asInt(), properties["RunPath"].asString());
	_animator->addAction("hurt", properties["HurtCount"].asInt(), properties["HurtPath"].asString(), 0.2, false);
	_animator->addAction("jump", properties["JumpCount"].asInt(), properties["JumpPath"].asString(), 0.1, false);
}

void Player::setSpeed(float value)
{
	//playerRun->setSpeed(value);
}
