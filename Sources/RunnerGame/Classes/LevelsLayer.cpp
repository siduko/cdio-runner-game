#include "LevelsLayer.h"


LevelsLayer::LevelsLayer()
{
	
}


LevelsLayer::~LevelsLayer()
{
}

Scene* LevelsLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(LevelsLayer::create());
	return scene;
}

bool LevelsLayer::init()
{
	if (!Layer::init())
		return false;

	auto wsize = Director::getInstance()->getWinSize();



	return true;
}
