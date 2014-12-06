#include "ChapterLayer.h"


ChapterLayer::ChapterLayer()
{
}


ChapterLayer::~ChapterLayer()
{
}

bool ChapterLayer::init()
{
	if (!Layer::init())
		return false;

	stringstream ss;
	string temp;

	auto wSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("Icons/background.png");
	bg->setPosition(ccp(wSize.width / 2, wSize.height / 2));
	this->addChild(bg);

	auto panelBorder = Sprite::create("Icons/BaseNewBorder.png");
	panelBorder->setPosition(ccp(wSize.width / 2, wSize.height / 2));

	auto chapterSelected = Sprite::create("Icons/ChapterSelect.png");
	chapterSelected->setPosition(ccp(wSize.width*0.5f, wSize.height*0.85f));

	ScrollView* scrollView = ScrollView::create();
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
	scrollView->setContentSize(Size(615, 369));
	scrollView->setBackGroundImage("Icons/BaseNew.png");
	scrollView->setClippingEnabled(true);
	scrollView->setPosition(ccp(wSize.width / 2, wSize.height / 2));
	scrollView->setAnchorPoint(ccp(0.5, 0.5));

	this->addChild(scrollView);
	this->addChild(panelBorder);
	this->addChild(chapterSelected);

	ValueVector chapters = DataController::getInstance()->getChapters();
	float lastPos = scrollView->getContentSize().width / 2;
	float padding = 0.0f;
	for (int i = 0; i < chapters.size();i++)
	{
		ValueMap chapter = chapters[i].asValueMap();
		Layout* layout = Layout::create();
		layout->setBackGroundImage("Chapters_chapterBackground.png");
		layout->setPosition(ccp(lastPos + padding, wSize.height*0.6));
		layout->setContentSize(Size(250,212));
		layout->setAnchorPoint(ccp(0.5, 0.5));

		Text* chapterName = Text::create(chapter["Name"].asString(), DataController::getInstance()->getGameSettings()["GameFont"].asString(), 32);
		chapterName->setPosition(ccp(layout->getContentSize().width / 2, layout->getContentSize().height*0.85));
		layout->addChild(chapterName);

		ImageView* chapterImage = ImageView::create(chapter["ChapterImage"].asString());
		chapterImage->setPosition(ccp(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
		layout->addChild(chapterImage);

		ss << DataController::getInstance()->getChapterScoreByIndex(i);
		ss >> temp;
		ss.clear();
		Text* chapterScore = Text::create(temp, DataController::getInstance()->getGameSettings()["GameFont"].asString(), 20);
		chapterScore->setPosition(ccp(layout->getContentSize().width * 0.5, layout->getContentSize().height*0.15));
		layout->addChild(chapterScore);

		/*ss << DataController::getInstance()->getChapterStarByIndex(i) << "/" << DataController::getInstance()->getChapterStarMaxByIndex(i);
		ss >> temp;
		ss.clear();
		Text* chapterStar = Text::create(temp, DataController::getInstance()->getGameSettings()["GameFont"].asString(), 20);
		chapterStar->setPosition(ccp(layout->getContentSize().width * 0.7, layout->getContentSize().height*0.17));
		layout->addChild(chapterStar);*/

		if (chapter["Locked"].asInt())
		{
			layout->setBackGroundImage("Chapters_chapterBackground_locked.png");
		}

		padding = 100;
		lastPos = layout->getPosition().x + layout->getContentSize().width;

		layout->setTouchEnabled(true);
		layout->addTouchEventListener([i](Ref *pSender, ui::Layout::TouchEventType type)
		{
			ValueMap chapter = DataController::getInstance()->getChapterByIndex(i);
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				UserDefault::getInstance()->setIntegerForKey("ChapterSelected", i);
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
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Audios/Mishief Stroll_bg.wav", true);
	return true;
}

Scene* ChapterLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(ChapterLayer::create());
	return scene;
}
