#ifndef PlayLayer_h__
#define PlayLayer_h__
#include "cocos2d.h"
#include "Player.h"
#include "Definitions.h"
#include "Utils.h"
#include "Item.h"
#include "HubLayer.h"
#include "GameoverLayer.h"
#include "GameObject.h"
#include "JoyStick.h"
USING_NS_CC;

class PlayLayer:public Layer
{
public:
	static Scene* createScene(string tmxpath);
	virtual bool init();
	bool createMap(string tmxpath);
	CREATE_FUNC(PlayLayer);
private:
	int score;

	PhysicsWorld* sceneWorld;
	TMXTiledMap* map;
	Size sceneSize;
	Player* player;

	inline void setPhysicsWorld(PhysicsWorld* word){ sceneWorld = word; }
	bool onContactBegin(PhysicsContact &contact);
	void setViewPointCenter(Point position);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	HubLayer* getHubLayer();
};
#endif // PlayLayer_h__