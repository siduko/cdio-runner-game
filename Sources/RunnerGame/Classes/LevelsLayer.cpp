#include "LevelsLayer.h"


LevelsLayer::LevelsLayer()
{

}


LevelsLayer::~LevelsLayer()
{
}

Scene* LevelsLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(LevelsLayer::create());
	return scene;
}

bool LevelsLayer::init()
{
	if (!Layer::init())
		return false;

	auto wsize = Director::getInstance()->getWinSize();

	stringstream ss;
	string temp;
	auto wSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("Icons/background.png");
	bg->setPosition(ccp(wSize.width / 2, wSize.height / 2));
	this->addChild(bg);


	auto lbLevelSelect = ImageView::create("Icons/Level_Select.png");
	lbLevelSelect->setPosition(ccp(wSize.width*0.5f, wSize.height*0.85f));

	auto panelBorder = Sprite::create("Icons/BaseNewBorder.png");
	panelBorder->setPosition(ccp(wSize.width / 2, wSize.height / 2));

	ScrollView* scrollView = ScrollView::create();
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
	scrollView->setContentSize(Size(624,369));
	scrollView->setClippingEnabled(true);
	scrollView->setBackGroundImage("Icons/BaseNew.png");
	scrollView->setPosition(ccp(wSize.width / 2, wSize.height / 2));
	scrollView->setAnchorPoint(ccp(0.5, 0.5));
	this->addChild(scrollView);
	this->addChild(panelBorder);
	this->addChild(lbLevelSelect);

	ValueVector levels = DataController::getInstance()->getLevelsInChapterByIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"));
	float lastPos = wSize.width / 2;
	float padding = 0.0f;
	for (int i = 0; i < levels.size(); i++)
	{
		ValueMap level = levels[i].asValueMap();
		Layout* layout = Layout::create();
		layout->setBackGroundImage("Icons/Base_1.png");
		layout->setPosition(ccp(lastPos + padding, wSize.height*0.6));
		layout->setContentSize(Size(82, 93));
		layout->setAnchorPoint(ccp(0.5, 0.5));

		Text* levelName = Text::create(level["Name"].asString(), DataController::getInstance()->getGameSettings()["GameFont"].asString(), 32);
		levelName->setPosition(ccp(layout->getContentSize().width / 2, layout->getContentSize().height*0.7));
		layout->addChild(levelName);

		ss << level["Score"].asString();
		ss >> temp;
		ss.clear();
		Text* levelScore = Text::create(temp, DataController::getInstance()->getGameSettings()["GameFont"].asString(), 13);
		levelScore->setPosition(ccp(layout->getContentSize().width * 0.5, layout->getContentSize().height*0.2));
		layout->addChild(levelScore);

		/*ss << level["Star"].asString() << "/" << level["StarMax"].asString();
		ss >> temp;
		ss.clear();
		Text* chapterStar = Text::create(temp, DataController::getInstance()->getGameSettings()["GameFont"].asString(), 20);
		chapterStar->setPosition(ccp(layout->getContentSize().width * 0.7, layout->getContentSize().height*0.17));
		layout->addChild(chapterStar);*/

		if (level["Locked"].asInt())
		{
			layout->setBackGroundImage("Icons/Color_Fill_41.png");
		}

		padding = 100;
		lastPos = layout->getPosition().x + layout->getContentSize().width;

		layout->setTouchEnabled(true);
		layout->addTouchEventListener([i](Ref *pSender, ui::Layout::TouchEventType type)
		{
			ValueMap level = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), i);
			ValueMap selectedLevel;
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				UserDefault::getInstance()->setIntegerForKey("LevelSelected", i); 
				selectedLevel = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"));
				if (selectedLevel["Locked"].asInt() == 0){
					Director::getInstance()->replaceScene(LoadingLayer::createScene());
				}
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

		scrollView->addChild(layout);
	}

	scrollView->setInnerContainerSize(Size(lastPos, wSize.height));

	auto btnBack = Button::create("Icons/Arrow_icon.png", "Icons/Arrow_icon.png", "Icons/Arrow_icon_disabled.png");
	btnBack->setPosition(ccp(wSize.width*0.5f, wSize.height*0.1f));
	btnBack->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
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
	this->addChild(btnBack);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Audios/Mishief Stroll_bg.wav", true);
	return true;
}
