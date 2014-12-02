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
	auto wSize = Director::getInstance()->getWinSize();

	auto coinIcon = ImageView::create("Icons/playerlayer_0000s_0005_hudCoin.png");
	coinIcon->setPosition(ccp(wSize.width*0.1f, wSize.height*0.95f));
	this->addChild(coinIcon);
	lbScore = Text::create("0", DataController::getInstance()->getGameSettings()["GameFont"].asString(), 30);
	lbScore->setPosition(ccp(wSize.width*0.2f, wSize.height*0.95f));
	lbScore->setColor(Color3B(241, 196, 15));
	this->addChild(lbScore);

	auto healthIcon = ImageView::create("Icons/playerlayer_0000s_0001_heart_full.png");
	healthIcon->setPosition(ccp(wSize.width*0.4f, wSize.height*0.95f));
	this->addChild(healthIcon);
	lbHealth = Text::create("0", DataController::getInstance()->getGameSettings()["GameFont"].asString(), 30);
	lbHealth->setPosition(ccp(wSize.width*0.5f, wSize.height*0.95f));
	lbHealth->setColor(Color3B(231, 76, 60));
	this->addChild(lbHealth);

	lbVelocity = Text::create("0 m/s", DataController::getInstance()->getGameSettings()["GameFont"].asString(), 30);

	auto avatar = Sprite::create("Icons/playerlayer_0000s_0000s_0003_avatar.png");
	auto meterBar = Sprite::create("Icons/playerlayer_0000s_0000s_0000_bar.png");
	powerJump = LoadingBar::create("Icons/playerlayer_0000s_0000s_0002_process.png");
	angleJump = Sprite::create("Icons/playerlayer_0000s_0000s_0001_needle.png");
	effectImage = ImageView::create("effectIcon1.png");
	lbEffectTimer = Text::create("00:00", DataController::getInstance()->getGameSettings()["GameFont"].asString(), 18);

	meterBar->setAnchorPoint(ccp(0, 0));
	meterBar->setPosition(ccp(wSize.width*0.01, wSize.height*0.01));
	avatar->setPosition(ccp(meterBar->getContentSize().width*0.12f, meterBar->getContentSize().height*0.42f));
	angleJump->setPosition(ccp(meterBar->getContentSize().width*0.12f, meterBar->getContentSize().height*0.42f));
	powerJump->setPosition(ccp(meterBar->getContentSize().width*0.25f, meterBar->getContentSize().height*0.42f));
	lbVelocity->setPosition(ccp(meterBar->getContentSize().width*0.4f, meterBar->getContentSize().height*0.8f));
	effectImage->setPosition(ccp(wSize.width*0.7f, wSize.height*0.93f));
	lbEffectTimer->setPosition(ccp(wSize.width*0.7f, wSize.height*0.85f));
	lbEffectTimer->setColor(Color3B(52, 73, 94));
	powerJump->setAnchorPoint(ccp(0, 0.5f));
	angleJump->setAnchorPoint(ccp(0,0.5));
	this->addChild(avatar);
	this->addChild(powerJump);
	this->addChild(angleJump);
	this->addChild(meterBar);
	this->addChild(lbVelocity);
	this->addChild(effectImage);
	this->addChild(lbEffectTimer);

	auto btnMenu = Button::create("Icons/Pause_icon.png", "Icons/Pause_icon.png", "Icons/Pause_icon_disabled.png");
	btnMenu->setName("btnMenu");
	btnMenu->setPosition(ccp(wSize.width*0.9f, wSize.height*0.9f));
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
	pausePanel->setPosition(ccp(wSize.width / 2, wSize.height / 2 - 500));
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
			this->getPlayer()->scheduleUpdate();
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
	pausePanel->addChild(btnReturnLevels);

	auto btnReplay = Button::create("Icons/Reload_icon.png", "Icons/Reload_icon.png", "Icons/Reload_icon.png");
	btnReplay->setPosition(ccp(pausePanelSize.width*0.2f, pausePanelSize.height*0.5f));
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
	pausePanel->addChild(btnReplay);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event){
		if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			this->getPlayer()->unscheduleUpdate();
			((Button*)this->getChildByName("btnMenu"))->setEnabled(false);
			((Layout*) this->getChildByName("PausePanel"))->runAction(Spawn::createWithTwoActions(FadeIn::create(0.5), EaseBounceIn::create(MoveBy::create(0.5, ccp(0, 500)))));
			((Layer*)this->getParent())->pause();
		}
	};
	keyboardListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event){

	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

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
		lbScore->setString(Utils::to_string(this->player->getScore()));
		EffectComponent* effectPlayer = (EffectComponent*)player->getEntityManager()->getComponentObjectByName("EffectComponent");
		if (effectPlayer->getAlive())
		{
			if (!effectImage->isVisible()){
				effectImage->setVisible(true);
				lbEffectTimer->setVisible(true);
				effectImage->loadTexture(effectPlayer->getEffectIcon());
				effectImage->runAction(Sequence::createWithTwoActions(DelayTime::create((int)(effectPlayer->getLifeTime()-2)), RepeatForever::create(Blink::create(1, 1))));
				
			}else
				lbEffectTimer->setText(Utils::count2Timer((int)effectPlayer->getLifeTime()));
		}else
		{
			if (effectImage->isVisible())
			{
				effectImage->setVisible(false);
				lbEffectTimer->setVisible(false);
			}
		}
		lbHealth->setText(Utils::to_string(player->getHealth()));
		if (player->getVelocity() < DataController::getInstance()->getGameSettings()["PlayerVelocityLimit"].asInt()*30/100)
			lbVelocity->setColor(Color3B(243, 156, 18));
		else if (player->getVelocity() < DataController::getInstance()->getGameSettings()["PlayerVelocityLimit"].asInt() * 60 / 100)
			lbVelocity->setColor(Color3B(211, 84, 0));
		else
			lbVelocity->setColor(Color3B(192, 57, 43));

	}
}