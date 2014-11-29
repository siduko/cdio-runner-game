#pragma once
#ifndef AboutLayer_h__
#define AboutLayer_h__
#include "cocos2d.h"
#include "MenuLayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class AboutLayer:public Layer
{
private:
	bool init();
public:
	AboutLayer();
	~AboutLayer();
	static Scene* createScene();
	CREATE_FUNC(AboutLayer);
};
#endif // AboutLayer_h__

