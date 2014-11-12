#ifndef EffectItem_h__
#define EffectItem_h__
#include "cocos2d.h"
#include "GameObject.h"
#include "Animator.h"
USING_NS_CC;

#pragma once
class EffectItem:public GameObject
{
public:
	EffectItem(ValueMap properties);
	~EffectItem();
	static EffectItem* create(ValueMap properties);
};
#endif // EffectItem_h__

