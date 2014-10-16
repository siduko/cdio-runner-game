#include "Egg.h"

Egg* Egg::create()
{
	Egg *sprite = new Egg();
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Egg::Egg()
{

}

Egg::~Egg()
{

}
