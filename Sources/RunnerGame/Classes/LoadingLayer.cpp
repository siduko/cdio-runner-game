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
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(LoadingLayer::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(LoadingLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	auto wSize = Director::getInstance()->getWinSize();
	string bgPath = "loadingBG" + Utils::to_string(Utils::randomValueBetween(1, 4)) + ".png";
	isLoaded = false;
	auto bg = Sprite::create(bgPath);
	bg->setPosition(ccp(wSize.width*0.5f, wSize.height*0.5f));
	bg->setContentSize(wSize);
	this->addChild(bg);

	auto loadingText = Text::create("Touch to continue...", DataController::getInstance()->getGameSettings()["GameFont"].asString(), 40);
	loadingText->setPosition(ccp(wSize.width*0.7f, wSize.height*0.05f));
	this->addChild(loadingText);
	loadingText->runAction(RepeatForever::create( Sequence::createWithTwoActions(FadeOut::create(3),FadeIn::create(2))));

	scheduleUpdate();

	return true;
}
void LoadingLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	isLoaded = true;
}
bool LoadingLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	isLoaded = true;
	return true;
}
void LoadingLayer::update(float dt)
{
	if(isLoaded){
	auto selectedLevel = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"));
	Director::getInstance()->replaceScene(PlayLayer::createScene(selectedLevel["TmxPath"].asString()));
	}
}
