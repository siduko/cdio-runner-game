#include "PlayLayer.h"


bool PlayLayer::init()
{
	if (!Layer::init())
		return false;
	player = NULL;
	sceneSize = Director::getInstance()->getWinSize();
	score = 0;
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayLayer::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	scheduleUpdate();

	return true;
}

Scene* PlayLayer::createScene(string tmxpath)
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(ccp(0, -WorldGravity));
	auto bglayer = Layer::create();
	auto bg = Sprite::create("bg.png");
	bg->setAnchorPoint(ccp(0, 0));
	bglayer->addChild(bg,1,1);
	scene->addChild(bglayer);
	auto hub = HubLayer::create();
	scene->addChild(hub, 3, 3);
	auto layer = PlayLayer::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer,2,2);
	layer->createMap(tmxpath);
	return scene;
}

bool PlayLayer::onContactBegin(PhysicsContact &contact)
{
	auto a = contact.getShapeA()->getBody();
	auto b = contact.getShapeB()->getBody();
	if (a->getCollisionBitmask() == CONTACT_PLAYER && b->getCollisionBitmask() == CONTACT_ITEM){
		score += ((Item*)b->getNode())->getScore();
		this->getHubLayer()->setScore("Score: " + Utils::to_string(score));
		b->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	if (b->getCollisionBitmask() == CONTACT_PLAYER && a->getCollisionBitmask() == CONTACT_ITEM){
		score += ((Item*)a->getNode())->getScore();
		this->getHubLayer()->setScore("Score: " + Utils::to_string(score));
		a->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	if (a->getCollisionBitmask() == CONTACT_PLAYER && b->getCollisionBitmask() == CONTACT_FLOOR || b->getCollisionBitmask() == CONTACT_PLAYER && a->getCollisionBitmask() == CONTACT_FLOOR){
		player->setAllowJump(true);
	}
	if (a->getCollisionBitmask() == CONTACT_PLAYER && b->getCollisionBitmask() == CONTACT_DieZone || b->getCollisionBitmask() == CONTACT_PLAYER && a->getCollisionBitmask() == CONTACT_DieZone){
		
		Director::getInstance()->replaceScene(GameoverLayer::createScene());
		return false;
	}
	if (a->getCollisionBitmask() == CONTACT_PLAYER && b->getCollisionBitmask() == CONTACT_EndGame || b->getCollisionBitmask() == CONTACT_PLAYER && a->getCollisionBitmask() == CONTACT_EndGame){
		Director::getInstance()->replaceScene(GameoverLayer::createScene());
	}
	return true;
}

bool PlayLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	player->jump(ccp(0, PlayerJump));
	((Animator*)player->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("jump",2.0f,false,true);
	return true;
}

bool PlayLayer::createMap(string tmxpath)
{
	map = TMXTiledMap::create(tmxpath);
	this->addChild(map);
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
			this->addChild(player);
			((Animator*)player->getEntityManager()->getComponentObjectByName("Animator"))->playActionByName("idle");
			player->move(ccp(5000, 0));
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
			this->addChild(node);
		}
		if (properties["type"].asString() == "Item")
		{
			auto item = Item::create(properties);
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

