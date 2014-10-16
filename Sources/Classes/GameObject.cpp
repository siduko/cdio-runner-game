#include "GameObject.h"


GameObject::GameObject()
{
	//initWithFile("star.png");
	auto t = new Texture2D();
	t->autorelease();
	initWithTexture(t);
	this->_animator = Animator::create();
	this->addChild(_animator);
}


GameObject::~GameObject()
{
}

void GameObject::setInfo(ValueMap properties)
{
	spriteBody = PhysicsBody::createBox(Size(properties["width"].asFloat(), properties["height"].asFloat()), PhysicsMaterial(1.0f, 0.0f, 0.0f));
	spriteBody->setDynamic(properties["DynamicBody"].asBool());
	spriteBody->setCollisionBitmask(properties["CollisionBitmask"].asInt());
	spriteBody->setContactTestBitmask(true);
	spriteBody->setRotationEnable(false);
	this->setPhysicsBody(spriteBody);
	this->setPosition(ccp(properties["x"].asFloat() + properties["width"].asFloat() / 2, properties["y"].asFloat() + properties["height"].asFloat() / 2));
}

GameObject* GameObject::create()
{
	GameObject *sprite = new GameObject();
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
