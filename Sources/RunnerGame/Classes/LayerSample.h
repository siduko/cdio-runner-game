#ifndef PlayLayer_h__
#define PlayLayer_h__
#include "cocos2d.h"
USING_NS_CC;

class PlayLayer:public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	CREATE_FUNC(PlayLayer);
};
#endif // PlayLayer_h__