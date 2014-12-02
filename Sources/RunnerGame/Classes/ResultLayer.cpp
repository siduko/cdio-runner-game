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

	if (UserDefault::getInstance()->getBoolForKey("LevelCompleted")){
		auto btnNext = Button::create("Icons/Forward_btn.png", "Icons/Forward_btn.png", "Icons/Forward_btn.png");
		btnNext->setPosition(ccp(resultPanel->getContentSize().width*0.75f, 0));
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
	}
	auto btnReturnLevel = Button::create("Icons/Menu_icon.png", "Icons/Menu_icon.png", "Icons/Menu_icon.png");
	btnReturnLevel->setPosition(ccp(resultPanel->getContentSize().width*0.25f, 0));
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

	auto btnReplay = Button::create("Icons/Reload_icon.png", "Icons/Reload_icon.png", "Icons/Reload_icon.png");
	btnReplay->setPosition(ccp(resultPanel->getContentSize().width*0.5f, 0));
	btnReplay->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
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

	auto lbScore = Text::create(s, DataController::getInstance()->getGameSettings()["GameFont"].asString(), 150);
	lbScore->setTextHorizontalAlignment(TextHAlignment::CENTER);
	lbScore->setPosition(ccp(resultPanel->getContentSize().width*0.5f, resultPanel->getContentSize().height*0.6f));
	lbScore->setColor(Color3B(215, 170, 116));
	resultPanel->addChild(lbScore);

	auto lbHighScore = Text::create("High Score\n" + Utils::to_string(UserDefault::getInstance()->getIntegerForKey("HighScore")), DataController::getInstance()->getGameSettings()["GameFont"].asString(), 32);
	lbHighScore->setTextHorizontalAlignment(TextHAlignment::CENTER);
	lbHighScore->setPosition(ccp(resultPanel->getContentSize().width*0.5f, resultPanel->getContentSize().height*0.3f));
	lbHighScore->setColor(Color3B(215, 170, 116));
	resultPanel->addChild(lbHighScore);

	return true;
}

Scene* ResultLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(ResultLayer::create());
	return scene;
}

