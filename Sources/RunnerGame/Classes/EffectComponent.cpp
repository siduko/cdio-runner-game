#include "EffectComponent.h"


EffectComponent::EffectComponent()
{
	_effectAnimation = NULL;
	_effectIcon = NULL;
	scheduleUpdate();
}


EffectComponent::~EffectComponent()
{
}

void EffectComponent::update(float dt)
{
	Player* _parent = (Player*)this->getParent();
	if (_parent)
	{
		if (_isAlive)
		{
			_lifeTime -= dt;
			if (_lifeTime < 0)
				_isAlive = false;
			else
			{
				switch (_runningEffect)
				{
				case SlowEffect:
					_parent->setVelocity(30);
					break;
				case UnlimitHealth:
					_parent->setHealth(_parent->getMaxHealth());
					break;
				}
			}
		}
		else
		{
			if (_effectAnimation)
				_parent->stopAction(_effectAnimation);
		}
	}
}

void EffectComponent::runEffect(EffectType type)
{
	switch (type)
	{
	case SlowEffect:
		_effectIcon = Sprite::create("sloweffecticon.png");

		_lifeTime = 10.0f;
		break;
	case UnlimitHealth:
		break;
	}
	_parent->runAction(_effectAnimation);
	_runningEffect = type;
	_isAlive = true;
}
