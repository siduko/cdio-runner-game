#include "Item.h"


Item::Item()
{
}


Item::~Item()
{
}

void Item::setInfo(ValueMap properties)
{
	GameObject::setInfo(properties);
	_score = properties["Score"].asInt();
	this->getAnimator()->addAction("idle", properties["ImageCount"].asInt(), properties["ImagePath"].asString());
	this->getAnimator()->playActionByName("idle");
}

Item* Item::create()
{
	Item *sprite = new Item();
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
