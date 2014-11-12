#include "EffectItem.h"


EffectItem::EffectItem(ValueMap properties):GameObject(properties)
{
	_entityManager->addComponentObject("Animator", Animator::create());
	Animator* animator = (Animator*)this->getEntityManager()->getComponentObjectByName("Animator");
	animator->addAction("idle", properties["ImageCount"].asInt(), properties["ImagePath"].asString());
	animator->playActionByName("idle", 1.0f, true);
}


EffectItem::~EffectItem()
{
}

EffectItem* EffectItem::create(ValueMap properties)
{
	EffectItem *sprite = new EffectItem(properties);
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
