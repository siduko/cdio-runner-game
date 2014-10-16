#include "PlayLayer.h"


bool PlayLayer::init()
{
	if (!Layer::init())
		return false;

	return true;
}

bool PlayLayer::onTouchBegan(Touch *touch, Event *unused_event)
{

}

Scene* PlayLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = PlayLayer::create();
	scene->addChild(layer);
	return scene;
}
