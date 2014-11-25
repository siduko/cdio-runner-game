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

	ScrollView* scrollView = ScrollView::create();
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
	scrollView->setContentSize(wSize);
	scrollView->setBackGroundImage("Icons/background.png");
	scrollView->setPosition(ccp(wSize.width / 2, wSize.height / 2));
	scrollView->setAnchorPoint(ccp(0.5, 0.5));
	this->addChild(scrollView);

	auto lbLevelSelect = ImageView::create("Icons/Level_Select.png");
	lbLevelSelect->setPosition(ccp(wSize.width*0.5f, wSize.height*0.9f));
	scrollView->addChild(lbLevelSelect);
	ValueVector levels = DataController::getInstance()->getLevelsInChapterByIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"));
	float lastPos = wSize.width / 2;
	float padding = 0.0f;
	for (int i = 0; i < levels.size(); i++)
	{
		ValueMap level = levels[i].asValueMap();
		Layout* layout = Layout::create();
		layout->setBackGroundImage("Chapters_chapterBackground.png");
		layout->setPosition(ccp(lastPos + padding, wSize.height / 2));
		layout->setContentSize(Size(288, 261));
		layout->setAnchorPoint(ccp(0.5, 0.5));

		Text* chapterName = Text::create(level["Name"].asString(), "fonts/Marker Felt.ttf", 32);
		chapterName->setPosition(ccp(layout->getContentSize().width / 2, layout->getContentSize().height*0.9));
		layout->addChild(chapterName);

		ss << level["Score"].asString();
		ss >> temp;
		ss.clear();
		Text* chapterScore = Text::create(temp, "fonts/Marker Felt.ttf", 20);
		chapterScore->setPosition(ccp(layout->getContentSize().width * 0.25, layout->getContentSize().height*0.17));
		layout->addChild(chapterScore);

		ss << level["Star"].asString() << "/" << level["StarMax"].asString();
		ss >> temp;
		ss.clear();
		Text* chapterStar = Text::create(temp, "fonts/Marker Felt.ttf", 20);
		chapterStar->setPosition(ccp(layout->getContentSize().width * 0.7, layout->getContentSize().height*0.17));
		layout->addChild(chapterStar);

		if (level["Locked"].asInt())
		{
			ImageView* chapterLock = ImageView::create("Chapters_lock.png");
			chapterLock->setPosition(ccp(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
			layout->addChild(chapterLock);
		}

		padding = 100;
		lastPos = layout->getPosition().x + layout->getContentSize().width;

		layout->setTouchEnabled(true);
		layout->addTouchEventListener([i](Ref *pSender, ui::Layout::TouchEventType type)
		{
			ValueMap level = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"));
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
	btnBack->setPosition(ccp(wSize.width*0.1f, wSize.height*0.1f));
	btnBack->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		auto chapterLayer = TransitionSlideInL::create(1, ChapterLayer::createScene());
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
	this->addChild(btnBack);

	return true;
}
