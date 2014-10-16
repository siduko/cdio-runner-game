#include "HubLayer.h"


HubLayer::HubLayer()
{
	player = NULL;
}


HubLayer::~HubLayer()
{
}

bool HubLayer::init()
{
	if (!Layer::init())
		return false;

	sceneSize = Director::getInstance()->getWinSize();

	auto avatar = Sprite::create("avatar.png");
	avatar->setPosition(ccp(sceneSize.width*0.1f, sceneSize.height*0.9f));
	this->addChild(avatar);

	lbScore = LabelTTF::create("Score: 0", "fonts/Marker Felt.ttf", 30, Size::ZERO, TextHAlignment::LEFT);
	lbScore->setColor(Color3B(255, 0, 0));
	lbScore->setFontFillColor(Color3B(255, 0, 0));
	lbScore->setFontSize(25);
	lbScore->setPosition(ccp(sceneSize.width*0.25f, sceneSize.height*0.95f));
	this->addChild(lbScore);

	lbVelocity = LabelTTF::create("0 m/s", "fonts/Marker Felt.ttf", 30,Size::ZERO,TextHAlignment::LEFT);
	lbVelocity->setColor(Color3B(255, 0, 0));
	lbVelocity->setFontFillColor(Color3B(255, 0, 0));
	lbVelocity->setFontSize(25);
	lbVelocity->setPosition(ccp(sceneSize.width*0.25f, sceneSize.height*0.85f));
	this->addChild(lbVelocity);

	auto joystick = JoyStick::create("joystick_bg.png", "joystick_ct.png");
	this->addChild(joystick, 100);
	joystick->setPosition(Vec2(100, 100));
	joystick->setDieRadius(60);
	joystick->onDirection = [this](JoyStickEnum dir, float vel){
		if (player)
		switch (dir)
		{
		case DEFAULT:
			player->getAnimator()->playActionByName("idle");
			break;
		case D_UP:
			break;
		case D_DOWN:
			break;
		case D_LEFT:
			player->move(ccp(-vel * 100, 0));
			player->setFlipX(true);
			player->getAnimator()->playActionByName("run");
			break;
		case D_RIGHT:
			player->move(ccp(vel * 100, 0));
			player->setFlipX(false);
			player->getAnimator()->playActionByName("run");
			break;
		case D_LEFT_UP:
			break;
		case D_LEFT_DOWN:
			break;
		case D_RIGHT_UP:
			break;
		case D_RIGHT_DOWN:
			break;
		default:
			break;
		}
	};
	joystick->onRun();

	scheduleUpdate();

	return true;
}

void HubLayer::setScore(string value)
{
	lbScore->setString(value);
}

void HubLayer::update(float delta)
{
	if (player)
	{
		lbVelocity->setString(Utils::to_string((int)this->player->spriteBody->getVelocity().x) + " m/s");
	}
}
