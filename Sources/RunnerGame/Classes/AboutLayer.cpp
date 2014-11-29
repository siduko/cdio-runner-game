#include "AboutLayer.h"


AboutLayer::AboutLayer()
{
}


AboutLayer::~AboutLayer()
{
}

bool AboutLayer::init()
{
	if (!Layer::init())
		return false;

	auto wSize = Director::getInstance()->getWinSize();
	auto bg = ImageView::create("aboutbg.png");
	bg->setPosition(ccp(wSize.width*0.5, wSize.height*0.5));
	this->addChild(bg);

	auto btnBack = Button::create("Icons/Arrow_icon.png", "Icons/Arrow_icon.png", "Icons/Arrow_icon_disabled.png");
	btnBack->setPosition(ccp(wSize.width*0.1f, wSize.height*0.9f));
	btnBack->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			Director::getInstance()->replaceScene(MenuLayer::createScene());
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
	this->addChild(btnBack);

	return true;
}

Scene* AboutLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = AboutLayer::create();
	scene->addChild(layer);
	return scene;
}
