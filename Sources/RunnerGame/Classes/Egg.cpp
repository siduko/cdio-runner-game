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
	initWithFile("CloseNormal.png");

	auto spriteBody = PhysicsBody::createCircle(this->getContentSize().width / 2,PhysicsMaterial(1.0f,0.5f,0.5f));
	spriteBody->setDynamic(true);
	spriteBody->setCollisionBitmask(CONTACT_EGG);
	spriteBody->setContactTestBitmask(true);
	this->setPhysicsBody(spriteBody);
}

Egg::~Egg()
{

}
