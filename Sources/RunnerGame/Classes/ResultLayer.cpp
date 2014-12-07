#include "ResultLayer.h"
#include "AdmobHelper.h"


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
	this->setKeyboardEnabled(true);
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



	auto resultLabel = ImageView::create("Icons/Level_Failed.png");
	resultLabel->setPosition(ccp(resultPanel->getContentSize().width*0.5f, resultPanel->getContentSize().height*0.9f));
	resultPanel->addChild(resultLabel);

	if (UserDefault::getInstance()->getBoolForKey("LevelCompleted")){
		auto resultLabelClear = ImageView::create("Icons/Level_Cleared.png");
			resultLabelClear->setPosition(ccp(resultPanel->getContentSize().width*0.5f, resultPanel->getContentSize().height*0.9f));
			resultPanel->addChild(resultLabelClear);

		auto btnNext = Button::create("Icons/Forward_btn.png", "Icons/Forward_btn.png", "Icons/Forward_btn.png");
		btnNext->setPosition(ccp(resultPanel->getContentSize().width*0.75f, 0));
		btnNext->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
		{

			if(UserDefault::getInstance()->getIntegerForKey("ChapterPred") == UserDefault::getInstance()->getIntegerForKey("ChapterSelected") && UserDefault::getInstance()->getIntegerForKey("LevelPred") == UserDefault::getInstance()->getIntegerForKey("LevelSelected")){

				if (UserDefault::getInstance()->getIntegerForKey("LevelSelected") + 1 < DataController::getInstance()->getLevelsInChapterByIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected")).size()){
					ValueMap nextLevel = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected") + 1);
					UserDefault::getInstance()->setIntegerForKey("LevelSelected", UserDefault::getInstance()->getIntegerForKey("LevelSelected") + 1);
				}
				else
				{
					if (UserDefault::getInstance()->getIntegerForKey("ChapterSelected") + 1 < DataController::getInstance()->getChapters().size()){
						ValueMap nextChapter = DataController::getInstance()->getChapterByIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected") + 1);
						ValueMap nextLevel = nextChapter["Levels"].asValueVector()[0].asValueMap();
						UserDefault::getInstance()->setIntegerForKey("ChapterSelected", UserDefault::getInstance()->getIntegerForKey("ChapterSelected") + 1);
						UserDefault::getInstance()->setIntegerForKey("LevelSelected", UserDefault::getInstance()->getIntegerForKey("LevelSelected") + 1);
					}
				}
			}

			ValueMap level = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"));
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				Director::getInstance()->replaceScene(LoadingLayer::createScene());
				//Director::getInstance()->replaceScene(PlayLayer::createScene(level["TmxPath"].asString()));
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
	//admod
		auto listener = EventListenerTouchOneByOne::create();

		listener->setSwallowTouches(true);

		listener->onTouchBegan = [](Touch* touch, Event* event)
		{
			if (AdmobHelper::isAdShowing)
				AdmobHelper::hideAd();
			else
				AdmobHelper::showAd();

			return true;

		};

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		//end admod
	auto btnReplay = Button::create("Icons/Reload_icon.png", "Icons/Reload_icon.png", "Icons/Reload_icon.png");
	btnReplay->setPosition(ccp(resultPanel->getContentSize().width*0.5f, 0));
	btnReplay->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			UserDefault::getInstance()->setIntegerForKey("ChapterSelected", UserDefault::getInstance()->getIntegerForKey("ChapterPred"));
			UserDefault::getInstance()->setIntegerForKey("LevelSelected", UserDefault::getInstance()->getIntegerForKey("LevelPred"));
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

	auto lbScore = Text::create(s, DataController::getInstance()->getGameSettings()["GameFont"].asString(), 100);
	lbScore->setTextHorizontalAlignment(TextHAlignment::CENTER);
	lbScore->setPosition(ccp(resultPanel->getContentSize().width*0.5f, resultPanel->getContentSize().height*0.6f));
	lbScore->setColor(Color3B(215, 170, 116));
	resultPanel->addChild(lbScore);

	auto lbHighScore = Text::create("High Score\n" + Utils::to_string(UserDefault::getInstance()->getIntegerForKey("HighScore")), DataController::getInstance()->getGameSettings()["GameFont"].asString(), 32);
	lbHighScore->setTextHorizontalAlignment(TextHAlignment::CENTER);
	lbHighScore->setPosition(ccp(resultPanel->getContentSize().width*0.5f, resultPanel->getContentSize().height*0.3f));
	lbHighScore->setColor(Color3B(215, 170, 116));
	resultPanel->addChild(lbHighScore);

	string strLevel = Utils::to_string(UserDefault::getInstance()->getIntegerForKey("ChapterPred")+1) + "-" + Utils::to_string(UserDefault::getInstance()->getIntegerForKey("LevelPred")+1);
	auto lbCurrentLevel = Text::create(strLevel, DataController::getInstance()->getGameSettings()["GameFont"].asString(), 30);
	lbCurrentLevel->setColor(Color3B(215, 170, 116));
	lbCurrentLevel->setTextHorizontalAlignment(TextHAlignment::CENTER);
	lbCurrentLevel->setPosition(ccp(resultPanel->getContentSize().width*0.5f, resultPanel->getContentSize().height*0.78f));
	resultPanel->addChild(lbCurrentLevel);

	return true;
}

void ResultLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
    	Director::getInstance()->replaceScene(LevelsLayer::createScene());
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_MENU)
    {
    }
}

Scene* ResultLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(ResultLayer::create());
	return scene;
}

