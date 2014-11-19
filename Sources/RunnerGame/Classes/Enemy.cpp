#include "Enemy.h"


Enemy::Enemy(ValueMap properties) :GameObject(properties)
{
	_entityManager->addComponentObject("Animator", Animator::create());
	Animator* animator = (Animator*)this->getEntityManager()->getComponentObjectByName("Animator");
	animator->addAction("idle", properties["ImageCount"].asInt(), properties["ImagePath"].asString());
	animator->addAction("moving", properties["MovingCount"].asInt(), properties["MovingPath"].asString());
	animator->addAction("dead", properties["DeadCount"].asInt(), properties["DeadPath"].asString());
	animator->addAction("behit", properties["BeHitCount"].asInt(), properties["BeHitPath"].asString());
	_enemyState = EnemyState::Moving;
	_enemyType = static_cast<EnemyType>(properties["EnemyType"].asInt());
	_actionTimeout = 0.0f;
	_moveTimeout = properties["MoveTimeout"].asFloat();
	_resetActionTimeout = false;
	_moveVelocity = properties["MoveVelocity"].asInt();
	_target = NULL;
}


Enemy::~Enemy()
{
}

Enemy* Enemy::create(ValueMap properties)
{
	Enemy *sprite = new Enemy(properties);
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Enemy::update(float dt)
{
	/*if (_target && ccpDistance(_target->getPosition(), this->getPosition()) <= DataController::getInstance()->getGameSettings()["TargetDetectedDistance"].asInt())
	{
		_enemyState = EnemyState::TargetDetected;
	}*/
	_actionTimeout -= dt;
	Animator* ani = (Animator*)this->getEntityManager()->getComponentObjectByName("Animator");
	switch (_enemyState)
	{
	case Idle:
		if (_resetActionTimeout){
			ani->playActionByName("idle", 0.2f, true);
			_actionTimeout = Utils::randomValueBetween(1.0f, 3.0f);
			_resetActionTimeout = false;
		}
		if (_actionTimeout<0)
		{
			_enemyState = EnemyState::Moving;
			_resetActionTimeout = true;
		}
		break;
	case Moving:
		if (_resetActionTimeout){
			ani->playActionByName("moving", 0.5f, true);
			_actionTimeout = Utils::randomValueBetween(1.0f, _moveTimeout);
			_resetActionTimeout = false;
			if (_moveVelocity > 0)
				this->setFlipX(true);
			else
				this->setFlipX(false);
		}
		if (_actionTimeout < 0){
			if (Utils::randomValueBetween(0, 1))
			{
				_enemyState = EnemyState::Idle;
			}
			_resetActionTimeout = true;
			_moveVelocity = -1 * _moveVelocity;
		}
		switch (_enemyType)
		{
		case Move:
			this->getPhysicsBody()->setVelocity(ccp(_moveVelocity, this->getPhysicsBody()->getVelocity().y));
			break;
		case Fly:
			this->getPhysicsBody()->setVelocity(ccp(_moveVelocity, this->getPhysicsBody()->getVelocity().y));
			break;
		case Stand:
			this->getPhysicsBody()->setVelocity(ccp(0, this->getPhysicsBody()->getVelocity().y));
			break;
		}
		break;
	case BeHit:
		if (_resetActionTimeout){
			ani->playActionByName("behit", 0.2f, true);
			_actionTimeout = 1.0f;
			_resetActionTimeout = false;
		}
		if (_actionTimeout < 0)
		{
			_resetActionTimeout = true;
			_enemyState = EnemyState::Idle;
		}
		break;
	case Dead:
		if (_resetActionTimeout){
			ani->playActionByName("dead", 0.2f, true);
			_actionTimeout = 0.5f;
			_resetActionTimeout = false;
			CCLOG("Enemy dead in %f", _actionTimeout);
		}
		if (_actionTimeout < 0)
		{
			/*_resetActionTimeout = true;
			_enemyState = EnemyState::Idle;*/
			CCLOG("Enemy Dead");
			this->removeFromParentAndCleanup(false);
		}
		break;
	case TargetDetected:
		if (_target)
		{
			if (_moveVelocity > 0)
				this->setFlipX(true);
			else
				this->setFlipX(false);
			this->getPhysicsBody()->setVelocity(ccpMult(Vec2::forAngle(ccpAngle(_target->getPosition(), this->getPosition())),abs(_moveVelocity)));
			//CCLOG("Enemy TargetDetected");
			if (ccpDistance(_target->getPosition(), this->getPosition()) > DataController::getInstance()->getGameSettings()["TargetDetectedDistance"].asInt())
				_enemyState = EnemyState::Moving;
		}

		break;
	}
}
