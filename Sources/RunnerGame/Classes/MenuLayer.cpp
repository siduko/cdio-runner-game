#include "MenuLayer.h"


bool MenuLayer::init()
{
	if (!Layer::init())
		return false;

	auto wSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("bgmenu.png");
	bg->setPosition(ccp(wSize.width*0.5, wSize.height*0.5));
	bg->setContentSize(wSize);
	this->addChild(bg);

	auto btnPlay = Button::create("Icons/Play_icon.png", "Icons/Play_icon.png", "Icons/Play_icon_disabled.png");
	btnPlay->setPosition(ccp(wSize.width*0.25, wSize.height*0.2));
	btnPlay->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		auto chapterLayer = TransitionCrossFade::create(1, ChapterLayer::createScene());
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			Director::getInstance()->replaceScene(chapterLayer);
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
	});
	this->addChild(btnPlay);

	return true;
}

Scene* MenuLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuLayer::create();
	scene->addChild(layer);
	return scene;
}

