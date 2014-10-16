#ifndef MenuLayer_h__
#define MenuLayer_h__
#include "cocos2d.h"
USING_NS_CC;

class MenuLayer:public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	void btnPlay_Click(Ref* pSender);
	void btnOption_Click(Ref* pSender);
	void btnQuit_Click(Ref* pSender);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	CREATE_FUNC(MenuLayer);
};
#endif // MenuLayer_h__
