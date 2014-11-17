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

	lbScore = Text::create("Score: 0", "fonts/Marker Felt.ttf", 30);
	lbScore->setPosition(ccp(sceneSize.width*0.25f, sceneSize.height*0.95f));
	this->addChild(lbScore);

	lbVelocity = Text::create("0 m/s", "fonts/Marker Felt.ttf", 30);
	lbVelocity->setColor(Color3B(255, 0, 0));
	lbVelocity->setPosition(ccp(sceneSize.width*0.25f, sceneSize.height*0.85f));
	this->addChild(lbVelocity);

	effectImage = ImageView::create("effectIcon1.png");
	effectImage->setPosition(ccp(sceneSize.width*0.5f, sceneSize.height*0.85f));
	this->addChild(effectImage);

	powerJump = LoadingBar::create("progress_texture.png");
	powerJump->setPosition(ccp(sceneSize.width*0.0f, sceneSize.height*0.0f));
	powerJump->setAnchorPoint(ccp(0, 0.5f));
	this->addChild(powerJump);
	powerJump->setVisible(false);

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
		lbVelocity->setString(Utils::to_string((int)this->player->getPhysicsBody()->getVelocity().x) + " m/s");
		lbScore->setString("Score: " + Utils::to_string(this->player->getScore()));
		EffectComponent* effectPlayer = (EffectComponent*)player->getEntityManager()->getComponentObjectByName("EffectComponent");
		effectImage->loadTexture(effectPlayer->getEffectIcon());
	}
}
