#include "SplashLayer.h"


Scene* SplashLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(SplashLayer::create());
	return scene;
}

bool SplashLayer::init()
{
	if (!Layer::init())
		return false;

	auto wSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("Icons/splashlayer_0000s_0001_bg.png");
	bg->setPosition(ccp(wSize.width*0.5, wSize.height*0.5));
	bg->setContentSize(wSize);
	this->addChild(bg);

	auto logo = Sprite::create("Icons/splashlayer_0000s_0000_logo.png");
	logo->setPosition(ccp(wSize.width*0.5, wSize.height*0.5));
	this->addChild(logo);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audios/playbg.wav");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audios/gameoverbg.wav");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audios/Mishief Stroll_bg.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Audios/jump1.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Audios/lose4.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Audios/coin10.wav");

	logo->runAction(Sequence::create(FadeOut::create(2), CallFunc::create([](){
		Director::getInstance()->replaceScene(MenuLayer::createScene());
	}), NULL));

	return true;
}
