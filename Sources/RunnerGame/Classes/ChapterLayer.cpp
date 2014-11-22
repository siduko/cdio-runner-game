#include "ChapterLayer.h"


Layers::ChapterLayer::ChapterLayer()
{
}


Layers::ChapterLayer::~ChapterLayer()
{
}

bool Layers::ChapterLayer::init()
{
	if (!Layer::init())
		return false;

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

	ValueVector chapters = DataController::getInstance()->getChapters();
	float lastPos = wSize.width / 2;
	float padding = 0.0f;
	for (int i = 0; i < chapters.size();i++)
	{
		ValueMap chapter = chapters[i].asValueMap();
		Layout* layout = Layout::create();
		layout->setBackGroundImage("Chapters_chapterBackground.png");
		layout->setPosition(ccp(lastPos + padding, wSize.height / 2));
		layout->setContentSize(Size(288,261));
		layout->setAnchorPoint(ccp(0.5, 0.5));

		Text* chapterName = Text::create(chapter["Name"].asString(), "fonts/Marker Felt.ttf", 32);
		chapterName->setPosition(ccp(layout->getContentSize().width / 2, layout->getContentSize().height*0.9));
		layout->addChild(chapterName);

		ImageView* chapterImage = ImageView::create(chapter["ChapterImage"].asString());
		chapterImage->setPosition(ccp(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
		layout->addChild(chapterImage);

		ss << DataController::getInstance()->getChapterScoreByIndex(i);
		ss >> temp;
		ss.clear();
		Text* chapterScore = Text::create(temp, "fonts/Marker Felt.ttf", 20);
		chapterScore->setPosition(ccp(layout->getContentSize().width * 0.25, layout->getContentSize().height*0.17));
		layout->addChild(chapterScore);

		ss << DataController::getInstance()->getChapterStarByIndex(i) << "/" << DataController::getInstance()->getChapterStarMaxByIndex(i);
		ss >> temp;
		ss.clear();
		Text* chapterStar = Text::create(temp, "fonts/Marker Felt.ttf", 20);
		chapterStar->setPosition(ccp(layout->getContentSize().width * 0.7, layout->getContentSize().height*0.17));
		layout->addChild(chapterStar);

		if (chapter["Locked"].asInt())
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
			ValueMap chapter;
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				UserDefault::getInstance()->setIntegerForKey("ChapterSelected", i);
				chapter = DataController::getInstance()->getChapterByIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"));
				if (chapter["Locked"].asInt()==0)
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

		scrollView->addChild(layout);
	}

	scrollView->setInnerContainerSize(Size(lastPos, wSize.height));

	auto btnBack = Button::create("Icons/Arrow_icon.png", "Icons/Arrow_icon.png", "Icons/Arrow_icon_disabled.png");
	btnBack->setPosition(ccp(wSize.width*0.1f, wSize.height*0.1f));
	btnBack->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
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
	this->addChild(btnBack);

	return true;
}

Scene* Layers::ChapterLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(ChapterLayer::create());
	return scene;
}
