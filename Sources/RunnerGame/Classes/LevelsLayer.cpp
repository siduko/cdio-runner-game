#include "LevelsLayer.h"


LevelsLayer::LevelsLayer()
{
	Vector<MenuItemSprite*> menuVector;
	auto wsize = Director::getInstance()->getWinSize();
	for (int i = 0; i < 60; i++)
	{
		Sprite* sprite = Sprite::create("CloseNormal.png");
		auto mi = MenuItemSprite::create(sprite,sprite, CC_CALLBACK_1(LevelsLayer::btnLevel_Clicked, this));
		mi->setAnchorPoint(ccp(1, 1));
		menuVector.pushBack(mi);
	}
	sliding = SlidingMenuGrid::menuWithArray(menuVector,5,3,ccp(0,0),ccp(100,100));
	sliding->setPosition(ccp(0,0));
	sliding->setAnchorPoint(ccp(0, 0));
	this->addChild(sliding);
	
}


LevelsLayer::~LevelsLayer()
{
}

Scene* LevelsLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(LevelsLayer::create());
	return scene;
}

void LevelsLayer::btnLevel_Clicked(Object* pSender)
{
	//string p = (string)((Node*)pSender)->getUserData();
	//Director::getInstance()->replaceScene(PlayLayer::createScene(p));
}

bool LevelsLayer::init()
{
	if (!Layer::init())
		return false;

	return true;
}
