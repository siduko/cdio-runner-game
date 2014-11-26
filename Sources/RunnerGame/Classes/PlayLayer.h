#ifndef PlayLayer_h__
#define PlayLayer_h__

#include "cocos2d.h"
#include "Player.h"
#include "DataController.h"
#include "Utils.h"
#include "Item.h"
#include "EffectItem.h"
#include "Enemy.h"
#include "GameObject.h"
#include "ResultLayer.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

class HubLayer;
class PlayLayer :public Layer
{
public:
	static Scene* createScene(string tmxpath, Color4B skyColor = Color4B(64, 62, 60, 254));
	virtual bool init();
	bool createMap(string tmxpath);
	CREATE_FUNC(PlayLayer);
private:
	PhysicsWorld* sceneWorld;
	TMXTiledMap* map;
	Size sceneSize;
	Player* player;
	Vec2 vecJump;
	inline void setPhysicsWorld(PhysicsWorld* word){ sceneWorld = word; }
	bool onContactBegin(PhysicsContact &contact);
	void setViewPointCenter(Point position);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	void update(float dt);
	HubLayer* getHubLayer();
};

#endif // PlayLayer_h__