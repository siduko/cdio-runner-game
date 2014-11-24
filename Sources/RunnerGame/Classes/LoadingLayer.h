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
	CREATE_FUNC(LoadingLayer);
};
#endif // LoadingLayer_h__

