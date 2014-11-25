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

	scheduleUpdate();

	return true;
}

Scene* PlayLayer::createScene(string tmxpath)
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(ccp(0, DataController::getInstance()->getGameSettings()["WorldGravity"].asInt()));
	auto bglayer = Layer::create();
	auto bg = Sprite::create("bg.png");
	bg->setAnchorPoint(ccp(0, 0));
	bglayer->addChild(bg, 1, 1);
	scene->addChild(bglayer);
	auto hub = HubLayer::create();
	scene->addChild(hub, 3, 3);
	auto layer = PlayLayer::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer, 2, 2);
	layer->createMap(tmxpath);
	return scene;
}

bool PlayLayer::onContactBegin(PhysicsContact &contact)
{
	auto a = contact.getShapeA()->getBody();
	auto b = contact.getShapeB()->getBody();

	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_ITEM"].asInt()){
		player->setScore(player->getScore() + ((Item*)b->getNode())->getScore());
		b->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	if (b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_ITEM"].asInt()){
		player->setScore(player->getScore() + ((Item*)a->getNode())->getScore());
		a->getNode()->removeFromParentAndCleanup(true);
		return false;
	}

	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_FLOOR"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_FLOOR"].asInt()){
		player->setAllowJump(true);
		if (player->getPlayerState() == PlayerState::Jumping)
			player->setPlayerState(PlayerState::Running);
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
			if (coldir == ccp(-1, 0) && coldir == ccp(1, 0))
			{
				ValueMap force = DataController::getInstance()->getGameSettings()["PlayerHurt"].asValueMap();
				player->getPhysicsBody()->applyImpulse(ccp(force["x"].asInt(), force["y"].asInt()));
				player->setPlayerState(PlayerState::Hurt);
				player->setActionTimeOut(2.0f);
			}
		}
		if (player->getPlayerState() == PlayerState::Jumping)
			player->setPlayerState(PlayerState::Running);
	}

	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_DieZone"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_DieZone"].asInt()){

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
		CCLOG("Collition with %d at: %f %f", enemy->getEnemyType(), coldir.x, coldir.y);
		if (coldir == ccp(1, 0) || coldir == ccp(-1, 0) || coldir == ccp(0, -1))
		{
			enemy->setResetActionTimeout(true);
			enemy->setEnemyState(Enemy::EnemyState::BeHit);
			ValueMap force = DataController::getInstance()->getGameSettings()["PlayerHurt"].asValueMap();
			player->getPhysicsBody()->applyImpulse(ccp(force["x"].asInt(), force["y"].asInt()));
			player->setPlayerState(PlayerState::Hurt);
			player->setActionTimeOut(2.0f);
		}
		if (coldir == ccp(0, 1))
		{
			enemy->setResetActionTimeout(true);
			enemy->setEnemyState(Enemy::EnemyState::Dead);
		}
		return true;
	}


	if (a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_EndGame"].asInt() || b->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt() && a->getCollisionBitmask() == DataController::getInstance()->getGameSettings()["CONTACT_EndGame"].asInt()){
		UserDefault::getInstance()->setIntegerForKey("Score", player->getScore());
		UserDefault::getInstance()->setIntegerForKey("Star", player->getScore());
		ValueMap level = DataController::getInstance()->getLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"));

		int selectedLevel = UserDefault::getInstance()->getIntegerForKey("LevelSelected");
		int selectedChapter = UserDefault::getInstance()->getIntegerForKey("ChapterSelected");

		if (level["Score"].asInt() < player->getScore()){
			level["Score"] = player->getScore();
			//CCLOG("Score %d", level)["Score"].asInt());
			DataController::getInstance()->setLevelByChapterIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"), UserDefault::getInstance()->getIntegerForKey("LevelSelected"), level);
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
				DataController::getInstance()->setChapterByIndex(UserDefault::getInstance()->getIntegerForKey("ChapterSelected"),nextChapter);
			}
		}
		DataController::getInstance()->saveGameData();
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
	}else if (vecJump.y < DataController::getInstance()->getGameSettings()["PlayerJump"].asInt())
	{
		player->jump(ccp(DataController::getInstance()->getGameSettings()["PlayerJump"].asInt()*0.2f, DataController::getInstance()->getGameSettings()["PlayerJump"].asInt()));
		player->setPlayerState(PlayerState::Jumping);
		((Animator*)player->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("jump", 2.0f, false, true);
		if (player->getVelocity() <= 0)
			player->setVelocity(30);
	}
	if (vecOut.x < 0){
		float velocityPlayer = player->getVelocity();
		if (velocityPlayer + vecOut.x >= 20)
			player->setVelocity(velocityPlayer + vecOut.x);
		else
			player->setVelocity(20);
	}
	this->getHubLayer()->powerJump->setPercent(0);
}

bool PlayLayer::createMap(string tmxpath)
{
	auto parallaxLayer = ParallaxNode::create();
	auto bg = Node::create();
	auto bg1 = Sprite::create("blue_desert.png");

	map = TMXTiledMap::create(tmxpath);
	float temp = 0;
	for (int i = 0; i*bg1->getContentSize().width < map->getMapSize().width;i++)
	{
		auto bg2 = Sprite::create("blue_desert.png");
		bg2->setPosition(ccp(i*bg1->getContentSize().width, bg1->getContentSize().height/2));
		bg->addChild(bg2);
		temp += bg1->getContentSize().width;
	}
	bg->setContentSize(Size(temp, bg1->getContentSize().height));
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
			this->addChild(player, 10, DataController::getInstance()->getGameSettings()["CONTACT_PLAYER"].asInt());
			((Animator*)player->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("idle");
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
	parallaxLayer->addChild(bg, -1, Vec2(map->getMapSize().width*0.8 / bg->getContentSize().width, map->getMapSize().height*0.8 / bg->getContentSize().height), Vec2::ZERO);
	parallaxLayer->addChild(map, 1, Vec2(1.0f, 1.0f), Vec2::ZERO);
	this->addChild(parallaxLayer);

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
}