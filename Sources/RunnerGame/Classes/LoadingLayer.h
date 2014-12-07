#ifndef LoadingLayer_h__
#define LoadingLayer_h__
#include "cocos2d.h"
#include "PlayLayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

#pragma once
class LoadingLayer:public Layer
{
public:
	LoadingLayer();
	static Scene* createScene();
	bool init();
	void update(float dt);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	CREATE_FUNC(LoadingLayer);
private:
	bool isLoaded;
};
#endif // LoadingLayer_h__

