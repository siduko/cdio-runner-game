#include "GameoverLayer.h"


GameoverLayer::GameoverLayer()
{
}


GameoverLayer::~GameoverLayer()
{
}

bool GameoverLayer::init()
{
	if (!Layer::init())
		return false;

	auto lb = LabelTTF::create("GameOver", "fonts/Marker Felt.ttf", 30);
	lb->setPosition(100, 200);
	this->addChild(lb);

	auto btnBack = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(GameoverLayer::btnBack_Click,this));
	btnBack->setPosition(ccp(200, 200));
	auto menu = Menu::create(btnBack, NULL);
	this->addChild(menu);

	return true;
}

Scene* GameoverLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(GameoverLayer::create());
	return scene;
}

void GameoverLayer::btnBack_Click(Ref* psender)
{
	Director::getInstance()->replaceScene(PlayLayer::createScene("Levels/level1.tmx"));
}
