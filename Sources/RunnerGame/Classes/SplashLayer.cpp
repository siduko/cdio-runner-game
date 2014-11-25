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

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audios/background.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("Audios/jump1.ogg");
	SimpleAudioEngine::getInstance()->preloadEffect("Audios/jump2.ogg");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Audios/background.mp3", true);

	logo->runAction(Sequence::create(FadeOut::create(2), CallFunc::create([](){
		auto scene = TransitionCrossFade::create(0.5, MenuLayer::createScene());
		Director::getInstance()->replaceScene(scene);
	}), NULL));

	return true;
}
