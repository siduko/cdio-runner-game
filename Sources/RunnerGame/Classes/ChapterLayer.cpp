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

	auto wSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("Chapters_Select-Episode.png");
	bg->setPosition(ccp(wSize.width / 2, wSize.height/2));
	this->addChild(bg,0);
	std::string path = FileUtils::getInstance()->fullPathForFilename("Levels/GameData.plist");
	ValueMap gameData = FileUtils::getInstance()->getValueMapFromFile(path);
	ValueVector chapters = gameData["Chapters"].asValueVector();
	CCLOG("%d", chapters.size());
	Vector<MenuItemSprite*> michapters;
	for (auto c : chapters)
	{
		Sprite* chapterBackground = Sprite::create("Chapters_chapterBackground.png");
		auto mi = MenuItemSprite::create(chapterBackground, chapterBackground, CC_CALLBACK_1(ChapterLayer::btnLevel_Clicked, this));
		mi->setContentSize(chapterBackground->getContentSize());
		ValueMap chapter = c.asValueMap();
		LabelTTF* lbName = LabelTTF::create(chapter["Name"].asString(), "arial", 32);
		lbName->setPosition(chapterBackground->getContentSize().width / 2, chapterBackground->getContentSize().height*0.9);
		mi->addChild(lbName);
		Sprite* chapterImage = Sprite::create(chapter["ChapterImage"].asString());
		chapterImage->setPosition(chapterBackground->getContentSize().width / 2, chapterBackground->getContentSize().height*0.6);
		mi->addChild(chapterImage);
		LabelTTF* lbScore = LabelTTF::create(chapter["Score"].asString(), "arial", 18);
		lbScore->setPosition(chapterBackground->getContentSize().width*0.2, chapterBackground->getContentSize().height*0.2);
		mi->addChild(lbScore);
		if (chapter["Locked"].asBool())
		{
			Sprite* lockSprite = Sprite::create("Chapters_lock.png");
			lockSprite->setPosition(chapterBackground->getContentSize().width / 2, chapterBackground->getContentSize().height / 2);
			mi->addChild(lockSprite);
		}
		michapters.pushBack(mi);
	}
	SlidingMenuGrid* menu = SlidingMenuGrid::menuWithArray(michapters, 1, 1, ccp(0, 0), ccp(10, 10), false);
	menu->setPosition(ccp(0, 0));
	menu->setAnchorPoint(ccp(0, 0));
	this->addChild(menu);

	return true;
}

Scene* ChapterLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(ChapterLayer::create());
	return scene;
}

void ChapterLayer::btnLevel_Clicked(Object* pSender)
{

}
