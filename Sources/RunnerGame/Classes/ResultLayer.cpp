#include "ResultLayer.h"


ResultLayer::ResultLayer()
{
}


ResultLayer::~ResultLayer()
{
}

bool ResultLayer::init()
{
	if (!Layer::init())
		return false;

	auto lb = LabelTTF::create("GameOver", "fonts/Marker Felt.ttf", 30);
	lb->setPosition(100, 200);
	this->addChild(lb);

	auto btnBack = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(ResultLayer::btnBack_Click,this));
	btnBack->setPosition(ccp(200, 200));
	auto menu = Menu::create(btnBack, NULL);
	this->addChild(menu);

	return true;
}

Scene* ResultLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(ResultLayer::create());
	return scene;
}

void ResultLayer::btnBack_Click(Ref* psender)
{
	Director::getInstance()->replaceScene(PlayLayer::createScene("Levels/level1.tmx"));
}
