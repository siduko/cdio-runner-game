#include "LoadingLayer.h"

LoadingLayer::LoadingLayer()
{
}

Scene* LoadingLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(LoadingLayer::create());
	return scene;
}

bool LoadingLayer::init()
{
	if (!Layer::init())
		return false;
	CCLOG("Init");
	auto wSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("loadingBG.png");
	bg->setPosition(ccp(wSize.width*0.5f, wSize.height*0.5f));
	bg->setContentSize(wSize);
	this->addChild(bg);

	auto loadingText = Sprite::create("loadingText.png");
	loadingText->setPosition(ccp(wSize.width*0.8f, wSize.height*0.05f));
	this->addChild(loadingText);
	loadingText->runAction(RepeatForever::create( Sequence::createWithTwoActions(FadeOut::create(3),FadeIn::create(2))));

	scheduleUpdate();

	return true;
}

void LoadingLayer::update(float dt)
{
	auto selectedLevel = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"));
	auto playLayer = PlayLayer::createScene(selectedLevel["TmxPath"].asString());
	Director::getInstance()->replaceScene(playLayer);
}
