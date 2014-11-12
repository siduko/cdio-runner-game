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
	auto layer = LevelsLayer::create();
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
	scrollView->setBackGroundImage("Chapters_Select-Episode.png");
	scrollView->setPosition(ccp(wSize.width / 2, wSize.height / 2));
	scrollView->setAnchorPoint(ccp(0.5, 0.5));
	this->addChild(scrollView);

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

		Text* chapterName = Text::create(level["Name"].asString(), "arial", 32);
		chapterName->setPosition(ccp(layout->getContentSize().width / 2, layout->getContentSize().height*0.9));
		layout->addChild(chapterName);

		ss << level["Score"].asString();
		ss >> temp;
		ss.clear();
		Text* chapterScore = Text::create(temp, "arial", 20);
		chapterScore->setPosition(ccp(layout->getContentSize().width * 0.25, layout->getContentSize().height*0.17));
		layout->addChild(chapterScore);

		ss << level["Star"].asString() << "/" << level["StarMax"].asString();
		ss >> temp;
		ss.clear();
		Text* chapterStar = Text::create(temp, "arial", 20);
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
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				UserDefault::getInstance()->setIntegerForKey("LevelSelected", i);
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

		scrollView->addChild(layout);
	}

	scrollView->setInnerContainerSize(Size(lastPos, wSize.height));

	return true;
}
