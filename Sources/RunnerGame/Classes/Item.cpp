#include "Item.h"


Item::Item(ValueMap properties) : GameObject(properties)
{
	_score = properties["Score"].asInt();
	_entityManager->addComponentObject("Animator", Animator::create());
	Animator* animator = (Animator*)this->getEntityManager()->getComponentObjectByName("Animator");
	animator->addAction("idle", properties["ImageCount"].asInt(), properties["ImagePath"].asString());
	animator->playActionByName("idle",1.0f,true);
}


Item::~Item()
{
}

Item* Item::create(ValueMap properties)
{
	Item *sprite = new Item(properties);
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
