#include "PlayLayer.h"
#include "HubLayer.h"

bool PlayLayer::init()
{
	if (!Layer::init())
		return false;
	player = NULL;
	sceneSize = Director::getInstance()->getWinSize();
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayLayer::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	UserDefault::getInstance()->setIntegerForKey("ChapterPred", UserDefault::getInstance()->getIntegerForKey("ChapterSelected"));
	UserDefault::getInstance()->setIntegerForKey("LevelPred", UserDefault::getInstance()->getIntegerForKey("LevelSelected"));
	UserDefault::getInstance()->setIntegerForKey("Score", 0);
	UserDefault::getInstance()->setIntegerForKey("HighScore", DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"))["Score"].asInt());
	UserDefault::getInstance()->setBoolForKey("LevelCompleted", false);
	int k = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"))["Score"].asInt();
	if (k>0) 
		UserDefault::getInstance()->setBoolForKey("LevelCompleted", true);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Audios/playbg.wav", true);

	scheduleUpdate();

	return true;
}




Scene* PlayLayer::createScene(string tmxpath, Color4B skyColor /*= Color4B(64, 62, 60, 254)*/)
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(ccp(0, DataController::getInstance()->getGameSettings()["WorldGravity"].asInt()));
	//auto bglayer = LayerColor::create(Color4B(204, 220,235,254));
	//bglayer->runAction(RepeatForever::create(Sequence::createWithTwoActions( TintTo::create(10, 64, 62, 60), TintTo::create(120, 204, 220, 235))));
	//scene->addChild(bglayer,1,1);
	auto hub = HubLayer::create();
	scene->addChild(hub, 3, 3);
	auto layer = PlayLayer::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer, 2, 2);
	layer->createMap(tmxpath);
	return scene;
}

bool PlayLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	vecJump = ccp(0, 0);
	return true;
}

void PlayLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	Vec2 vecOut = ccpSub(touch->getLocation(), touch->getStartLocation());
	vecJump = ccpMult(vecOut, DataController::getInstance()->getGameSettings()["JumpStep"].asInt());
	float angleJump = ccpAngle(vecOut, ccp(1, 0)) * 180 / 3.14f;
	if (vecJump.x > DataController::getInstance()->getGameSettings()["JumpLimit"].asFloat() || vecJump.y > DataController::getInstance()->getGameSettings()["JumpLimit"].asFloat()){
		vecJump.x = DataController::getInstance()->getGameSettings()["JumpLimit"].asFloat();
		vecJump.y = DataController::getInstance()->getGameSettings()["JumpLimit"].asFloat();
	}
	this->getHubLayer()->powerJump->setPercent(vecJump.y*100.0f / DataController::getInstance()->getGameSettings()["JumpLimit"].asFloat());
	this->getHubLayer()->angleJump->setRotation(angleJump);
}

void PlayLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	Vec2 vecOut = ccpSub(touch->getLocation(), touch->getStartLocation());
	if (vecJump.y > DataController::getInstance()->getGameSettings()["PlayerJump"].asInt() && vecJump.x >= 0){
		player->jump(vecJump);
		player->setPlayerState(PlayerState::Jumping);
		((Animator*)player->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("jump", 2.0f, false, true);
		if (player->getVelocity() <= 0)
			player->setVelocity(30); 
		this->addEffect("smoke", "Effects/wind%d.png", 11, player->getPosition() - ccp(0, player->getBoundingBox().size.height*0.3), 1);
		SimpleAudioEngine::getInstance()->playEffect("Audios/jump1.wav");
	}else if (vecJump.y < DataController::getInstance()->getGameSettings()["PlayerJump"].asInt())
	{
		player->jump(ccp(DataController::getInstance()->getGameSettings()["PlayerJump"].asInt()*0.5f, DataController::getInstance()->getGameSettings()["PlayerJump"].asInt()));
		player->setPlayerState(PlayerState::Jumping);
		((Animator*)player->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("jump", 2.0f, false, true);
		if (player->getVelocity() <= 0)
			player->setVelocity(30); 
		this->addEffect("smoke", "Effects/wind%d.png", 11, player->getPosition() - ccp(0, player->getBoundingBox().size.height*0.3), 1);
		SimpleAudioEngine::getInstance()->playEffect("Audios/jump1.wav");
	}
	if (vecOut.x < 0){
		float velocityPlayer = player->getVelocity();
		if (velocityPlayer + vecOut.x >= 20)
			player->setVelocity(velocityPlayer + vecOut.x);
		else
			player->setVelocity(20);
	}
	
	player->setPlayerState(PlayerState::Running);
	this->getHubLayer()->powerJump->setPercent(0);
}

bool PlayLayer::createMap(string tmxpath)
{
	auto parallaxLayer = ParallaxNode::create();
	auto bg = Node::create();
	string bgPath = "bg" + Utils::to_string(Utils::randomValueBetween(1, 8)) + ".png";
	auto bg1 = Sprite::create(bgPath);
	auto bg1width = bg1->getContentSize().width;

	map = TMXTiledMap::create(tmxpath);
	parallaxLayer->addChild(bg, -100, Vec2(0.5f, 0.5f), Vec2::ZERO);
	parallaxLayer->addChild(map, 1, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(parallaxLayer, -10);
	//this->addChild(map);
	auto sizemap = map->getMapSize().width*map->getTileSize().width;
	float temp = 0;
	for (int i = 0; i*bg1width < sizemap; i++)
	{
		auto bg2 = Sprite::create(bgPath);
		bg2->setAnchorPoint(ccp(0, 0));
		bg2->setPosition(ccp(i*bg1width, 0));
		bg->addChild(bg2);
		temp += bg1width;
	}
	bg->setContentSize(Size(temp, 0));
	CCTMXObjectGroup *objectGroup = map->objectGroupNamed("Objects");
	if (objectGroup == NULL){
		this->getHubLayer()->setScore("Level Error");
		return false;
	}
	for (Value object : objectGroup->getObjects())
	{
		auto properties = object.asValueMap();
		if (properties["type"].asString() == "Player")
		{
			player = Player::create(properties);
			player->setAcceleration(DataController::getInstance()->getGameSettings()["PlayerAcceleration"].asInt());
			this->addChild(player, 10, DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt());
			//((Animator*)player->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("idle");
			this->getHubLayer()->setPlayer(player);
		}
		if (properties["type"].asString() == "Floor")
		{
			auto node = Node::create();
			node->setPosition(ccp(properties["x"].asFloat() + properties["width"].asFloat() / 2, properties["y"].asFloat() + properties["height"].asFloat() / 2));
			auto spriteBody = PhysicsBody::createBox(Size(properties["width"].asFloat(), properties["height"].asFloat()), PhysicsMaterial(1.0f, 0.5f, 0.5f));
			spriteBody->setDynamic(false);
			spriteBody->setCollisionBitmask(properties["CollisionBitmask"].asInt());
			spriteBody->setContactTestBitmask(true);
			node->setPhysicsBody(spriteBody);
			node->setRotation(properties["Rotation"].asFloat());
			this->addChild(node);
		}
		if (properties["type"].asString() == "Block")
		{
			auto node = Node::create();
			node->setPosition(ccp(properties["x"].asFloat() + properties["width"].asFloat() / 2, properties["y"].asFloat() + properties["height"].asFloat() / 2));
			auto spriteBody = PhysicsBody::createBox(Size(properties["width"].asFloat(), properties["height"].asFloat()), PhysicsMaterial(1.0f, 0.5f, 0.5f));
			spriteBody->setDynamic(properties["DynamicBody"].asBool());
			spriteBody->setCollisionBitmask(properties["CollisionBitmask"].asInt());
			spriteBody->setContactTestBitmask(true);
			node->setPhysicsBody(spriteBody);
			node->setRotation(properties["Rotation"].asFloat());
			this->addChild(node);
		}
		if (properties["type"].asString() == "Enemy")
		{
			auto node = Enemy::create(properties);
			this->addChild(node);
			node->setTarget(player);
		}
		if (properties["type"].asString() == "Item")
		{
			auto item = Item::create(properties);
			this->addChild(item);
		}
		if (properties["type"].asString() == "EffectItem")
		{
			auto item = EffectItem::create(properties);
			this->addChild(item);
		}
		if (properties["type"].asString() == "DieZone")
		{
			auto node = Node::create();
			node->setPosition(ccp(properties["x"].asFloat() + properties["width"].asFloat() / 2, properties["y"].asFloat() + properties["height"].asFloat() / 2));
			auto spriteBody = PhysicsBody::createBox(Size(properties["width"].asFloat(), properties["height"].asFloat()), PhysicsMaterial(1.0f, 0.5f, 0.5f));
			spriteBody->setDynamic(properties["DynamicBody"].asBool());
			spriteBody->setCollisionBitmask(properties["CollisionBitmask"].asInt());
			spriteBody->setContactTestBitmask(true);
			node->setPhysicsBody(spriteBody);
			this->addChild(node);
		}
		if (properties["type"].asString() == "EndGame")
		{
			auto node = GameObject::create(properties);
			this->addChild(node);
		}
	}
	return true;
}

bool PlayLayer::onContactBegin(PhysicsContact &contact)
{
	auto a = contact.getShapeA()->getBody();
	auto b = contact.getShapeB()->getBody();

	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_ITEM"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_ITEM"].asInt()){

		if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt()){
			this->addEffect("blast", "Effects/blast%d.png", 9, b->getNode()->getPosition(), 1);
			player->setScore(player->getScore() + ((Item*)b->getNode())->getScore());
			b->getNode()->removeFromParentAndCleanup(true);
		}
		else
		{
			this->addEffect("blast", "Effects/blast%d.png", 9, a->getNode()->getPosition(), 1);
			player->setScore(player->getScore() + ((Item*)a->getNode())->getScore());
			a->getNode()->removeFromParentAndCleanup(true);
		}
		SimpleAudioEngine::getInstance()->playEffect("Audios/coin10.wav");
		return false;
	}

	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_BLOCK"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_BLOCK"].asInt()){
		player->setAllowJump(true);
		if (player->getPlayerState() == PlayerState::Running)
		{
			Vec2 coldir;
			if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt())
				coldir = Utils::collisedDirection(a->getNode()->getBoundingBox(), b->getNode()->getBoundingBox());
			else
				coldir = Utils::collisedDirection(b->getNode()->getBoundingBox(), a->getNode()->getBoundingBox());
			CCLOG("Collition at: %f %f", coldir.x, coldir.y);
			if (coldir == ccp(-1, 0) || coldir == ccp(1, 0))
			{
				player->setPlayerState(PlayerState::Hurt);
				player->setActionTimeOut(1.0f);
			}
		}
		if (player->getPlayerState() == PlayerState::Jumping)
			player->setPlayerState(PlayerState::Running);
		return true;
	}

	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_FLOOR"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_FLOOR"].asInt()){
		player->setAllowJump(true);
		if (player->getPlayerState() == PlayerState::Jumping){
			this->addEffect("smoke", "Effects/wind%d.png", 11, player->getPosition() - ccp(0, player->getBoundingBox().size.height*0.3),1);
		}
		player->setPlayerState(PlayerState::Running);
		Vec2 coldir;
		if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt())
			coldir = Utils::collisedDirection(a->getNode()->getBoundingBox(), b->getNode()->getBoundingBox());
		else
			coldir = Utils::collisedDirection(b->getNode()->getBoundingBox(), a->getNode()->getBoundingBox());
		if (coldir == ccp(-1, 0) || coldir == ccp(1, 0))
		{
			player->setPlayerState(PlayerState::Hurt);
			player->setActionTimeOut(1.0f);
		}
		return true;
	}

	

	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_DieZone"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_DieZone"].asInt()){
		UserDefault::getInstance()->setIntegerForKey("Score", player->getScore());
		UserDefault::getInstance()->setIntegerForKey("Star", player->getScore());
		Director::getInstance()->replaceScene(ResultLayer::createScene());
		return false;
	}

	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_ENEMY"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_ENEMY"].asInt()){
		Enemy* enemy;
		Vec2 coldir;
		if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_ENEMY"].asInt()){
			enemy = (Enemy*)a->getNode();
			coldir = Utils::collisedDirection(b->getNode()->getBoundingBox(), a->getNode()->getBoundingBox());
		}
		else
		{
			enemy = (Enemy*)b->getNode();
			coldir = Utils::collisedDirection(a->getNode()->getBoundingBox(), b->getNode()->getBoundingBox());
		}
		EffectComponent* effectPlayer = (EffectComponent*)player->getEntityManager()->getComponentObjectByName("EffectComponent");
		if (effectPlayer->getAlive())
		{
			if (effectPlayer->getRunningEffect() == EffectType::UnlimitHealth)
			{ 
				this->addEffect("lightningclaw", "Effects/lightningclaw%d.png", 16, enemy->getPosition(), 1);
				enemy->setResetActionTimeout(true);
				enemy->setEnemyState(Enemy::EnemyState::Dead);
				SimpleAudioEngine::getInstance()->playEffect("Audios/lose4.wav");
				return false;
			}
		}
		if (coldir == ccp(1, 0) || coldir == ccp(-1, 0) || coldir == ccp(0, -1))
		{
			enemy->setResetActionTimeout(true);

			enemy->setEnemyState(Enemy::EnemyState::BeHit);
			player->setPlayerState(PlayerState::Hurt);
			player->setActionTimeOut(1.0f);
			player->setHealth(player->getHealth() - 1);
			if (player->getHealth() <= 0)
			{
				UserDefault::getInstance()->setIntegerForKey("Score", player->getScore());
				Director::getInstance()->replaceScene(ResultLayer::createScene());
			}
		}
		if (coldir == ccp(0, 1))
		{
			this->addEffect("lightningclaw", "Effects/lightningclaw%d.png", 16, enemy->getPosition(), 1);
			enemy->setResetActionTimeout(true);
			enemy->setEnemyState(Enemy::EnemyState::Dead);
			SimpleAudioEngine::getInstance()->playEffect("Audios/lose4.wav");
			player->setScore(player->getScore() + 500);
		}
		return false;
	}


	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_EndGame"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_EndGame"].asInt()){
		UserDefault::getInstance()->setIntegerForKey("Score", player->getScore());
		UserDefault::getInstance()->setIntegerForKey("Star", player->getScore());
		ValueMap level = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"));
		
		int selectedLevel = UserDefault::getInstance()->getIntegerForKey("LevelSelected");
		int selectedChapter = UserDefault::getInstance()->getIntegerForKey("ChapterSelected");
		UserDefault::getInstance()->setBoolForKey("LevelCompleted", true);
		if (level["Score"].asInt() < player->getScore()){
			level["Score"] = player->getScore();
			//CCLOG("Score %d", level)["Score"].asInt());
			DataController::getInstance()->setLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"), level);
			UserDefault::getInstance()->setIntegerForKey("HighScore", player->getScore());
		}

		if (selectedLevel + 1 < DataController::getInstance()->getLevelsInChapterByIndex(selectedChapter).size()){
			ValueMap nextLevel = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected") + 1);
			nextLevel["Locked"] = 0;
			UserDefault::getInstance()->setIntegerForKey("LevelSelected", UserDefault::getInstance()->getIntegerForKey("LevelSelected") + 1);
			DataController::getInstance()->setLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"), nextLevel);
		}
		else
		{
			if (UserDefault::getInstance()->getIntegerForKey("ChapterSelected") + 1 < DataController::getInstance()->getChapters().size()){
				ValueMap nextChapter = DataController::getInstance()->getChapterByIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected") + 1);
				nextChapter["Locked"] = 0;
				ValueMap nextLevel = nextChapter["Levels"].asValueVector()[0].asValueMap();
				nextLevel["Locked"] = 0;
				UserDefault::getInstance()->setIntegerForKey("ChapterSelected", UserDefault::getInstance()->getIntegerForKey("ChapterSelected") + 1);
				UserDefault::getInstance()->setIntegerForKey("LevelSelected", UserDefault::getInstance()->getIntegerForKey("LevelSelected") + 1);
				DataController::getInstance()->setChapterByIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), nextChapter);
			}
		}
		DataController::getInstance()->saveGameData();
		//UserDefault::getInstance()->setIntegerForKey("HighScore", DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"))["Score"].asInt());
		Director::getInstance()->replaceScene(ResultLayer::createScene());
		return false;
	}

	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_EffectItem"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_EffectItem"].asInt()){
		EffectComponent* effectPlayer = (EffectComponent*)player->getEntityManager()->getComponentObjectByName("EffectComponent");
		if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_EffectItem"].asInt())
			a->getNode()->removeFromParentAndCleanup(true);
		else
			b->getNode()->removeFromParentAndCleanup(true);
		effectPlayer->runRandomEffect();
		return false;
	}



	return true;
}

void PlayLayer::setViewPointCenter(Point position)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (map->getMapSize().width * map->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (map->getMapSize().height * map->getTileSize().height) - winSize.height / 2);
	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
}

HubLayer* PlayLayer::getHubLayer()
{
	return (HubLayer*)this->getParent()->getChildByTag(3);
}

void PlayLayer::update(float dt)
{
	this->setViewPointCenter(player->getPosition());
	if (player->getPlayerState() == PlayerState::Running && abs(player->getVelocity())>300)
		this->addEffect("smoke", "Effects/smokeEffect%d.png", 11, player->getPosition() - ccp(0, player->getBoundingBox().size.height*0.5), dt*abs( player->getVelocity()) / 20);
}

void PlayLayer::addEffect(string name, string imagePath, int imageCount, Point pos, float timeRemove)
{
	GameObject* effect = GameObject::create();
	effect->setPosition(pos);
	Animator* ani = Animator::create();
	ani->addAction(name, imageCount, imagePath);
	effect->getEntityManager()->addComponentObject("Animator", ani);
	ani->playActionByName(name, timeRemove, true);
	effect->runAction(Sequence::createWithTwoActions(DelayTime::create(timeRemove), CallFunc::create([effect](){
		effect->removeFromParentAndCleanup(true);
	})));
	this->addChild(effect);
}
