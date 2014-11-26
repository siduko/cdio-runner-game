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

	auto winSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("Icons/background.png");
	bg->setPosition(ccp(winSize.width*0.5f, winSize.height*0.5f));
	this->addChild(bg);

	auto resultPanel = ui::Layout::create();
	resultPanel->setBackGroundImage("Icons/Base-006.png");
	resultPanel->setClippingEnabled(false);
	resultPanel->setContentSize(Size(479, 347));
	resultPanel->setPosition(ccp(winSize.width*0.5f, winSize.height*0.5f));
	resultPanel->setAnchorPoint(ccp(0.5, 0.5));
	this->addChild(resultPanel);

	auto resultLabel = ImageView::create("Icons/Level_Cleared.png");
	resultLabel->setPosition(ccp(resultPanel->getContentSize().width*0.5f, resultPanel->getContentSize().height));
	resultPanel->addChild(resultLabel);

	auto btnNext = Button::create("Icons/Forward_btn.png", "Icons/Forward_btn.png", "Icons/Forward_btn.png");
	btnNext->setPosition(ccp(resultPanel->getContentSize().width*0.8f, resultPanel->getContentSize().height*0.2f));
	btnNext->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		ValueMap level = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"));
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			Director::getInstance()->replaceScene(PlayLayer::createScene(level["TmxPath"].asString()));
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
	resultPanel->addChild(btnNext);

	auto btnReturnLevel = Button::create("Icons/Menu_icon.png", "Icons/Menu_icon.png", "Icons/Menu_icon.png");
	btnReturnLevel->setPosition(ccp(resultPanel->getContentSize().width*0.2f, resultPanel->getContentSize().height*0.2f));
	btnReturnLevel->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			Director::getInstance()->replaceScene(LevelsLayer::createScene());
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
	resultPanel->addChild(btnReturnLevel);

	auto btnReturnMenu = Button::create("star.png", "star.png", "star.png");
	btnReturnMenu->setPosition(ccp(resultPanel->getContentSize().width*0.6f, resultPanel->getContentSize().height*0.2f));
	btnReturnMenu->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
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
	resultPanel->addChild(btnReturnMenu);

	auto btnReplay = Button::create("Icons/Reload_icon.png", "Icons/Reload_icon.png", "Icons/Reload_icon.png");
	btnReplay->setPosition(ccp(resultPanel->getContentSize().width*0.4f, resultPanel->getContentSize().height*0.2f));
	btnReplay->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		//ValueMap level = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterPred"), UserDefault::getInstance()->getIntegerForKey("LevelPred"));
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			//Director::getInstance()->replaceScene(PlayLayer::createScene(level["TmxPath"].asString()));
			Director::getInstance()->replaceScene(LoadingLayer::createScene());
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
	resultPanel->addChild(btnReplay);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Audios/gameoverbg.wav", true);

	int score = UserDefault::getInstance()->getIntegerForKey("Score");
	stringstream ss;
	ss << score;
	string s;
	ss >> s;
	auto lbScore = Text::create(s, "fonts/Marker Felt.ttf", 32);
	lbScore->setPosition(ccp(resultPanel->getContentSize().width*0.5f, resultPanel->getContentSize().height*0.5f));
	resultPanel->addChild(lbScore);

	return true;
}

Scene* ResultLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(ResultLayer::create());
	return scene;
}

