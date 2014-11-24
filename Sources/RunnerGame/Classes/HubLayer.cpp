#include "HubLayer.h"
#include "PlayLayer.h"

HubLayer::HubLayer()
{
	player = NULL;
}


HubLayer::~HubLayer()
{
}

bool HubLayer::init()
{
	if (!Layer::init())
		return false;
	sceneSize = Director::getInstance()->getWinSize();

	

	auto avatar = Sprite::create("avatar.png");
	avatar->setPosition(ccp(sceneSize.width*0.1f, sceneSize.height*0.9f));
	this->addChild(avatar);

	lbScore = Text::create("Score: 0", "fonts/Marker Felt.ttf", 30);
	lbScore->setPosition(ccp(sceneSize.width*0.25f, sceneSize.height*0.95f));
	this->addChild(lbScore);

	lbVelocity = Text::create("0 m/s", "fonts/Marker Felt.ttf", 30);
	lbVelocity->setColor(Color3B(255, 0, 0));
	lbVelocity->setPosition(ccp(sceneSize.width*0.25f, sceneSize.height*0.85f));
	this->addChild(lbVelocity);

	effectImage = ImageView::create("effectIcon1.png");
	effectImage->setPosition(ccp(sceneSize.width*0.5f, sceneSize.height*0.85f));
	this->addChild(effectImage);

	powerJump = LoadingBar::create("progress_texture.png");
	powerJump->setPosition(ccp(sceneSize.width*0.0f, sceneSize.height*0.0f));
	powerJump->setAnchorPoint(ccp(0, 0.5f));
	this->addChild(powerJump);
	powerJump->setVisible(false);

	auto btnMenu = Button::create("Icons/Pause_icon.png", "Icons/Pause_icon.png", "Icons/Pause_icon_disabled.png");
	btnMenu->setName("btnMenu");
	btnMenu->setPosition(ccp(sceneSize.width*0.8f, sceneSize.height*0.85f));
	btnMenu->addTouchEventListener([this](Ref *pSender, ui::Button::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			this->getPlayer()->unscheduleUpdate();
			((Button*)this->getChildByName("btnMenu"))->setEnabled(false);
			((Layout*) this->getChildByName("PausePanel"))->runAction(Spawn::createWithTwoActions(FadeIn::create(0.5), EaseBounceIn::create(MoveBy::create(0.5, ccp(0, 500)))));
			((Layer*)this->getParent())->pause();
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
	this->addChild(btnMenu);

	auto pausePanel = Layout::create();
	pausePanel->setBackGroundImage("Icons/Shape_4_copy_2.png");
	pausePanel->setContentSize(Size(359, 179));
	pausePanel->setPosition(ccp(sceneSize.width / 2, sceneSize.height / 2 - 500));
	pausePanel->setAnchorPoint(ccp(0.5, 0.5));
	pausePanel->setName("PausePanel");
	this->addChild(pausePanel);

	auto pausePanelSize = pausePanel->getContentSize();

	auto pauseLabel = ImageView::create("Icons/Paused.png");
	pauseLabel->setPosition(ccp(pausePanelSize.width / 2, pausePanelSize.height));
	pausePanel->addChild(pauseLabel);

	auto btnClosePause = Button::create("Icons/Shape_1.png", "Icons/Shape_1.png", "Icons/Shape_1_disabled.png");
	btnClosePause->setPosition(ccp(pausePanelSize.width, pausePanelSize.height));
	btnClosePause->addTouchEventListener([this](Ref *pSender, ui::Button::TouchEventType type)
	{
		Layout* pausePanel = (Layout*) this->getChildByName("PausePanel");
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			this->getPlayer()->unscheduleUpdate();
			((Button*)this->getChildByName("btnMenu"))->setEnabled(true);
			pausePanel->runAction(Spawn::createWithTwoActions(FadeOut::create(0.5), EaseBounceOut::create(MoveBy::create(0.5, ccp(0, -500)))));
			((Layer*)this->getParent())->resume();
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
	pausePanel->addChild(btnClosePause);

	auto btnMusic = Button::create("Icons/Speaker_icon.png", "Icons/Speaker_icon.png", "Icons/Speaker_icon_disabled.png");
	btnMusic->setPosition(ccp(pausePanelSize.width*0.6f, pausePanelSize.height*0.5f));
	btnMusic->addTouchEventListener([this](Ref *pSender, ui::Button::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			UserDefault::getInstance()->setBoolForKey("MusicEnabled", !UserDefault::getInstance()->getBoolForKey("MusicEnabled", true));
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume((float)UserDefault::getInstance()->getBoolForKey("MusicEnabled", true));
			if (UserDefault::getInstance()->getBoolForKey("MusicEnabled", true))
				((Button*)pSender)->loadTextureNormal("Icons/Speaker_icon.png");
			else
				((Button*)pSender)->loadTextureNormal("Icons/Speaker_icon_disabled.png");
			CCLOG("%d %f", UserDefault::getInstance()->getBoolForKey("MusicEnabled", true), SimpleAudioEngine::getInstance()->getBackgroundMusicVolume());
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
	pausePanel->addChild(btnMusic);

	auto btnEffect = Button::create("Icons/Music_tone_icon.png", "Icons/Music_tone_icon.png", "Icons/Music_tone_icon_disabled.png");
	btnEffect->setPosition(ccp(pausePanelSize.width*0.8f, pausePanelSize.height*0.5f));
	btnEffect->addTouchEventListener([this](Ref *pSender, ui::Button::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			UserDefault::getInstance()->setBoolForKey("EffectEnabled", !UserDefault::getInstance()->getBoolForKey("EffectEnabled", true));
			SimpleAudioEngine::getInstance()->setEffectsVolume((float)UserDefault::getInstance()->getBoolForKey("EffectEnabled", true));
			if (UserDefault::getInstance()->getBoolForKey("EffectEnabled", true))
				((Button*)pSender)->loadTextureNormal("Icons/Music_tone_icon.png");
			else
				((Button*)pSender)->loadTextureNormal("Icons/Music_tone_icon_disabled.png");
			CCLOG("%d %f", UserDefault::getInstance()->getBoolForKey("EffectEnabled", true), SimpleAudioEngine::getInstance()->getEffectsVolume());
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
	pausePanel->addChild(btnEffect);

	auto btnReturnLevels = Button::create("Icons/Menu_icon.png", "Icons/Menu_icon.png", "Icons/Menu_icon.png");

	btnReturnLevels->setPosition(ccp(pausePanelSize.width*0.4f, pausePanelSize.height*0.5f));
	btnReturnLevels->addTouchEventListener([this](Ref *pSender, ui::Button::TouchEventType type)
	{
		auto levelLayer = TransitionSlideInL::create(1, LevelsLayer::createScene());
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			Director::getInstance()->replaceScene(levelLayer);
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
	pausePanel->addChild(btnReturnLevels);

	auto btnReplay = Button::create("Icons/Reload_icon.png", "Icons/Reload_icon.png", "Icons/Reload_icon.png");
	btnReplay->setPosition(ccp(pausePanelSize.width*0.2f, pausePanelSize.height*0.5f));
	btnReplay->addTouchEventListener([](Ref *pSender, ui::Button::TouchEventType type)
	{
		auto loadingLayer = TransitionSlideInR::create(1, LoadingLayer::createScene());
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			Director::getInstance()->replaceScene(loadingLayer);
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
	pausePanel->addChild(btnReplay);

	scheduleUpdate();

	return true;
}

void HubLayer::setScore(string value)
{
	lbScore->setString(value);
}

void HubLayer::update(float delta)
{
	if (player)
	{
		lbVelocity->setString(Utils::to_string((int)this->player->getPhysicsBody()->getVelocity().x) + " m/s");
		lbScore->setString("Score: " + Utils::to_string(this->player->getScore()));
		EffectComponent* effectPlayer = (EffectComponent*)player->getEntityManager()->getComponentObjectByName("EffectComponent");
		effectImage->loadTexture(effectPlayer->getEffectIcon());
	}
}