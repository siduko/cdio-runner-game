#include "MenuLayer.h"
#include "AboutLayer.h"


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
	btnPlay->setName("BtnPlay");
	btnPlay->setPosition(ccp(wSize.width*0.25, wSize.height*0.15));
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

	auto btnAbout = Button::create("Icons/_icon.png", "Icons/_icon.png", "Icons/_icon_disabled.png");
	btnAbout->setName("BtnPlay");
	btnAbout->setPosition(ccp(wSize.width*0.85, wSize.height*0.15));
	btnAbout->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			Director::getInstance()->replaceScene(AboutLayer::createScene());
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
	this->addChild(btnAbout);

	auto btnQuit = Button::create("Icons/Shape_1.png", "Icons/Shape_1.png", "Icons/Shape_1_disabled.png");
	btnQuit->setName("btnQuit");
	btnQuit->setPosition(ccp(wSize.width*0.9f, wSize.height*0.9f));
	btnQuit->addTouchEventListener([this](Ref *pSender, ui::Button::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			((Button*)this->getChildByName("BtnPlay"))->setEnabled(false);
			((Button*)this->getChildByName("btnQuit"))->setEnabled(false);
			(Layout*) this->getChildByName("ExitPanel")->runAction(FadeIn::create(0.5));
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
	this->addChild(btnQuit);

	auto exitPanel = Layout::create();
	exitPanel->setBackGroundImage("Icons/Base-005.png");
	exitPanel->setName("ExitPanel");
	exitPanel->setContentSize(Size(391, 147));
	exitPanel->setAnchorPoint(ccp(0.5, 0.5));
	exitPanel->setPosition(ccp(wSize.width*0.5, wSize.height*0.5));
	exitPanel->setOpacity(0);
	this->addChild(exitPanel);

	auto exitText = ImageView::create("Icons/Do_you_really_want__to_quit.png");
	exitText->setPosition(ccp(exitPanel->getContentSize().width*0.5, exitPanel->getContentSize().height*0.5));
	exitPanel->addChild(exitText);

	auto btnOk = Button::create("Icons/Color_Fill_2.png", "Icons/Color_Fill_2.png", "Icons/Color_Fill_2_disabled.png");
	btnOk->setPosition(ccp(exitPanel->getContentSize().width*0.25, 0));
	exitPanel->addChild(btnOk);
	btnOk->addTouchEventListener([this](Ref *pSender, ui::Button::TouchEventType type){
		Director::getInstance()->end();
	});

	auto btnCancel = Button::create("Icons/Shape_1.png", "Icons/Shape_1.png", "Icons/Shape_1_disabled.png");
	btnCancel->setPosition(ccp(exitPanel->getContentSize().width*0.75, 0));
	exitPanel->addChild(btnCancel);
	btnCancel->addTouchEventListener([this](Ref *pSender, ui::Button::TouchEventType type){
		(Layout*) this->getChildByName("ExitPanel")->runAction(FadeOut::create(0.5));
		((Button*)this->getChildByName("BtnPlay"))->setEnabled(true);
		((Button*)this->getChildByName("btnQuit"))->setEnabled(true);
	});

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event){
		if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			((Button*)this->getChildByName("BtnPlay"))->setEnabled(false);
			(Layout*) this->getChildByName("ExitPanel")->runAction(FadeIn::create(0.5));
		}
	};
	keyboardListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event){

	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Audios/Mishief Stroll_bg.wav", true);
	return true;
}

Scene* MenuLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuLayer::create();
	scene->addChild(layer);
	return scene;
}

