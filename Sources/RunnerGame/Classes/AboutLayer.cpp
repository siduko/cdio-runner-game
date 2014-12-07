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
	this->setKeypadEnabled(true);
	auto wSize = Director::getInstance()->getWinSize();
	auto bg = ImageView::create("aboutbg.png");
	bg->setPosition(ccp(wSize.width*0.5, wSize.height*0.5));
	this->addChild(bg);

	auto btnBack = Button::create("Icons/Arrow_icon.png", "Icons/Arrow_icon.png", "Icons/Arrow_icon_disabled.png");
	btnBack->setPosition(ccp(wSize.width*0.1f, wSize.height*0.85f));
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
void AboutLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
    	Director::getInstance()->replaceScene(MenuLayer::createScene());
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_MENU)
    {
    }
}
Scene* AboutLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = AboutLayer::create();
	scene->addChild(layer);
	return scene;
}
