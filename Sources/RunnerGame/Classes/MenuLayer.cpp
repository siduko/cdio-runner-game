#include "MenuLayer.h"

namespace Layers{

	bool MenuLayer::init()
	{
		if (!Layer::init())
			return false;

		auto wSize = Director::getInstance()->getWinSize();

		auto btnPlay = Button::create("Icons/Play_icon.png", "Icons/Play_icon.png", "Icons/Play_icon_disabled.png");
		btnPlay->setPosition(ccp(wSize.width*0.5, wSize.height*0.3));
		btnPlay->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				Director::getInstance()->replaceScene(ChapterLayer::createScene());
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

}