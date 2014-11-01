#ifndef EffectComponent_h__
#define EffectComponent_h__
#include "cocos2d.h"
#include "ComponentObject.h"
#include "Player.h"
USING_NS_CC;
enum EffectType
{
	SlowEffect,
	UnlimitHealth
};
class EffectComponent:public ComponentObject
{
private:
	void update(float dt);
	Action* _effectAnimation;
	EffectType _runningEffect;
public:
	CC_SYNTHESIZE(Sprite*, _effectIcon, EffectIcon);
	CC_SYNTHESIZE(float, _lifeTime, LifeTime);
	CC_SYNTHESIZE(bool, _isAlive, Alive);
	void runEffect(EffectType type);
	EffectComponent();
	~EffectComponent();
};
#endif // EffectComponent_h__