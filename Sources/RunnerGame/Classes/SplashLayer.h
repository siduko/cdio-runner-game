#ifndef SplashLayer_h__
#define SplashLayer_h__
#include "cocos2d.h"
#include "MenuLayer.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

#pragma once
class SplashLayer: public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SplashLayer);
};
#endif // SplashLayer_h__