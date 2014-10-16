#include "MenuLayer.h"


bool MenuLayer::init()
{
	if (!Layer::init())
		return false;

	auto btnPlay = MenuItemImage::create("CloseNormal.png","CloseSelected.png",menu_selector(MenuLayer::btnPlay_Click));
	auto btnOption = MenuItemImage::create("CloseNormal.png","CloseSelected.png",menu_selector(MenuLayer::btnOption_Click));
	auto btnQuit = MenuItemImage::create("CloseNormal.png","CloseSelected.png",menu_selector(MenuLayer::btnQuit_Click));

	Menu* menu = Menu::create(btnPlay,btnOption,btnQuit);
	menu->alignItemsVerticallyWithPadding(20);
	this->addChild(menu);
	return true;
}

void MenuLayer::btnPlay_Click(Ref* pSender) {
}

void MenuLayer::btnOption_Click(Ref* pSender) {
}

void MenuLayer::btnQuit_Click(Ref* pSender) {
}

bool MenuLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

Scene* MenuLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuLayer::create();
	scene->addChild(layer);
	return scene;
}
