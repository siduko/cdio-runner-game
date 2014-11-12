#ifndef EffectComponent_h__
#define EffectComponent_h__
#include "cocos2d.h"
#include "ComponentObject.h"
#include "Player.h"
USING_NS_CC;
using namespace std;
enum EffectType
{
	None,
	SlowEffect,
	FastEffect,
	UnlimitHealth
};
class EffectComponent:public ComponentObject
{
private:
	void update(float dt);
	GameObject* _effectObject;
public:
	CC_SYNTHESIZE(string, _effectIcon, EffectIcon);
	CC_SYNTHESIZE(float, _lifeTime, LifeTime);
	CC_SYNTHESIZE(bool, _isAlive, Alive);
	CC_SYNTHESIZE(EffectType, _runningEffect, RunningEffect);
	static EffectComponent* create();
	void runEffect(EffectType type);
	void runRandomEffect();
	EffectComponent();
	~EffectComponent();
	virtual void onEnter();
};
#endif // EffectComponent_h__