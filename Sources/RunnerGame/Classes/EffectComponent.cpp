#include "EffectComponent.h"


EffectComponent::EffectComponent()
{
	_effectIcon = "";
	_isAlive = false;
	_lifeTime = 0;
	_runningEffect = EffectType::None;

	scheduleUpdate();
}


EffectComponent::~EffectComponent()
{
	//_effectObject->removeFromParentAndCleanup(true);
}

void EffectComponent::update(float dt)
{
	Player* _parent = (Player*)this->getParent();
	if (_parent)
	{
		if (_isAlive)
		{
			//if (_effectObject)
			//	_effectObject->setVisible(true);
			_lifeTime -= dt;
			if (_lifeTime < 0){
				_isAlive = false;
				//_runningEffect = None;
			}
			else
			{
				switch (_runningEffect)
				{
				case SlowEffect:
					_parent->setVelocity(30);
					_parent->setAutoControlVelocity(false);
					break;
				case FastEffect:
					_parent->setVelocity(500);
					_parent->setAutoControlVelocity(false);
					break;
				case UnlimitHealth:
					_parent->setHealth(_parent->getMaxHealth());
					break;
				}
			}
		}
		else
		{
			switch (_runningEffect)
			{
			case None:
				break;
			case SlowEffect:
				_parent->setVelocity(100);
				_parent->setAutoControlVelocity(true);
				_runningEffect = None;
				break;
			case FastEffect:
				_parent->setVelocity(100);
				_parent->setAutoControlVelocity(true);
				_runningEffect = None;
				break;
			case UnlimitHealth:
				break;
			}
			//if (_effectObject)
			//	_effectObject->setVisible(false);
			_effectIcon = "effectIcon1.png";
		}
	}
}

void EffectComponent::runEffect(EffectType type)
{
	switch (type)
	{
	case SlowEffect:
		_effectIcon = "effectIcon25.png";
		_lifeTime = 10.0f;
		break;
	case FastEffect:
		_effectIcon = "effectIcon13.png";
		_lifeTime = 5.0f;
		break;
	case UnlimitHealth:
		_effectIcon = "effectIcon21.png";
		_lifeTime = 5.0f;
		break;
	}
	CCLOG("%d",_runningEffect);
	_runningEffect = type;
	_isAlive = true;
}

void EffectComponent::runRandomEffect()
{
	runEffect(static_cast<EffectType>(rand() % EffectType::None));
}

EffectComponent* EffectComponent::create()
{
	EffectComponent *node = new EffectComponent();
	if (node)
	{
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}

void EffectComponent::onEnter()
{
	//_effectObject = GameObject::create();
	//this->getParent()->addChild(_effectObject);
}
